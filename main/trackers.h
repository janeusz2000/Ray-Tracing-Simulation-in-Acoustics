#ifndef TRACKERS_H
#define TRACKERS_H

#include "core/classUtlilities.h"
#include "core/ray.h"
#include "main/model.h"
#include "main/resultsCalculation.h"
#include "nlohmann/json.hpp"
#include "obj/objects.h"

#include <fstream>
#include <sstream>
#include <string_view>
#include <unordered_map>

// Contains objects and functions that are responsible for exporting calculated
// data, objects and ray trajectories in simulation to different files. They
// are used for visualization and debugging purposes.
namespace trackers {

using Json = nlohmann::json;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

// Holds all simulated resutls in simulation and prepares data for export as
// json file;
class ResultTracker : Printable {
public:
  void registerResult(std::string_view parameterName,
                      const std::map<float, float> &result);

  // Saves acoustic in json format
  void saveRaport(std::string path) const;
  // TODO: create file association with right reference and trained model
  // TODO: put this implementation outside of this class
  // void compareDataToReference(std::string_view path);

  void printItself(std::ostream &os) const noexcept override;
  const Json generateRaport();

private:
  Json raport_;

  std::unordered_map<std::string_view, std::map<float, float>> results_;
};

// Mediator between different type of input data File.
// Prepares data to be saved into file
struct FileBuffer {
  // clears buffer and acquires data from json to buffer
  void acquireJsonFile(const Json &json);
  std::stringstream stream;
};

// Mediator between std::ofstream and FileBuffer.
// Manages opening, saving, writing and closing file.
class FileInterface : public Printable {
public:
  virtual ~FileInterface(){};
  virtual void openFileWithOverwrite() = 0;
  virtual void write(const FileBuffer &buffer) = 0;
  virtual void writeWithoutFlush(const FileBuffer &buffer) = 0;
  void printItself(std::ostream &os) const noexcept override;

private:
  virtual void open() = 0;
};

// Mediator between std::ofstream and FileBuffer.
// Manages opening, saving, writing and closing file.
class File : public FileInterface {
public:
  explicit File(std::string_view path = "") : path_(path.data()){};
  virtual ~File(){};
  // Opens file at given |path| overwriting existing one.
  // Throws std::invalid_argument exception if file is not found
  // at given path.
  void openFileWithOverwrite();
  // Opens file at given |path| without overwriting it.
  // Data is appended after existing one. Throws
  // std::invalid_argument exception if file is not found
  // at given path.
  // Writes given FileBuffer into file and flush content;
  void write(const FileBuffer &buffer);
  void writeWithoutFlush(const FileBuffer &buffer);
  void printItself(std::ostream &os) const noexcept override;

  void setPath(std::string_view path);

private:
  void open();
  void handleErrors();

protected:
  std::ofstream fileStream_;
  std::string path_;
};

// contains utilities for rendering javascript syntax in FileBuffer
namespace javascript {

FileBuffer initVar(std::string_view variableName);
FileBuffer initLet(std::string_view variableName);
FileBuffer initConst(std::string_view variableName);
FileBuffer initArray();
FileBuffer endArray();
FileBuffer initObject();
FileBuffer endObject();

void addCommaInBuffer(FileBuffer &buffer);
void endLineInBuffer(FileBuffer &buffer);
void initArrayInBuffer(FileBuffer &buffer);
void endArrayInBuffer(FileBuffer &buffer);
void initObjectInBuffer(FileBuffer &buffer);
void endObjectInBuffer(FileBuffer &buffer);
void initConstInBuffer(FileBuffer &buffer, std::string_view constName);
} // namespace javascript

// Enable loading screen by setting loading variable to true;
void startSimulation();
// Disable loading screen by setting loading variable to false;
void endSimulation();

// Represents Energy Collected |first| at certain time |second|
using EnergyPerTime = std::unordered_map<float, float>;
// Represent collected energy value from each collector
// at Collectors at the same index.
using Energies = std::vector<EnergyPerTime>;
// Represent collected energy from collectors at the given float that
// represent frequency of the simulation.
using EnergyPerFrequency = std::unordered_map<float, Energies>;

// Contains basic utilities for saving models and saving results of the
// simulation.
struct DataExporter : Printable {
  // Save results of the simulation at given |path| as results.js file, with
  // json structure. if |referenceModel| is false, file will be overwritten.
  void saveResultsAsJson(std::string_view path,
                         const EnergyPerFrequency &results,
                         bool referenceModel = false);

  // Overwrites |model| to the given .js file as Json file in javascript syntax
  // as const model variable or if the reference model is given true, data is
  // appended as a const referenceModel variable.
  void saveModelToJson(std::string_view pathToFolder, ModelInterface *model,
                       bool referenceModel = false);

  void printItself(std::ostream &os) const noexcept override;

private:
  Json convertTriangleToJson(const objects::TriangleObj &triangle) const;
};
// Tracks all reached position by rays in the simulation and
// saves them to files in the given path.
class PositionTrackerInterface : public Printable {
public:
  virtual ~PositionTrackerInterface(){};
  static void checkStreamIfGood(const std::ofstream &stream);
  virtual void initializeNewFrequency(float frequency) = 0;
  virtual void initializeNewTracking() = 0;
  virtual void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) = 0;
  virtual void endCurrentFrequency() = 0;
  virtual void endCurrentTracking() = 0;
  virtual void save() = 0;
  virtual void switchToReferenceModel() = 0;
  void printItself(std::ostream &os) const noexcept override;
};

// Used for running simulation without any visual representation.
class FakePositionTracker : public PositionTrackerInterface {
public:
  void initializeNewFrequency(float frequency) override{};
  void initializeNewTracking() override{};
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override{};
  void endCurrentFrequency() override{};
  void endCurrentTracking() override{};
  void save() override{};
  void switchToReferenceModel() override{};
  void printItself(std::ostream &os) const noexcept override;
};

// Tracks all reached rays position and saves them to js file as json data at
// given path. REQUIREMENTS: file must exist at given path.
class JsonPositionTracker : public PositionTrackerInterface {
public:
  JsonPositionTracker(std::string_view path);

  void initializeNewFrequency(float frequency) override;
  void initializeNewTracking() override;
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void endCurrentFrequency() override;
  void endCurrentTracking() override;

  void save() override;
  void switchToReferenceModel() override;
  void printItself(std::ostream &os) const noexcept override;

private:
  std::ofstream outFile_;
  std::vector<core::RayHitData> currentTracking_;
  File file_;
  FileBuffer FileBuffer_;
};

// Performs sampling of trackings and acquires them into given .js file as json
// data. |numOfRaysSquared| represents how many rays tracking will be in the
// simulation. Note: Overall number of ray tracking is |numOfRaysSquared|^2
// |numOfVisibleRaysSquared| represents how many trackings will be accumulated
// by trackers. Note: Overall number of accumulated trackings is
// |numOfVisibleRaysSquared|^2.
// REQUIREMENTS: File at |path| must exist, |numOfRaysSquared| and
// |numOfVisibleRaysSquared| must be > 0
class JsonSampledPositionTracker : public PositionTrackerInterface {
public:
  JsonSampledPositionTracker(std::string_view path, int numOfRaysSquared,
                             int numOfVisibleRaysSquared);

  void initializeNewFrequency(float frequency) override;
  void initializeNewTracking() override;
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void endCurrentFrequency() override;
  void endCurrentTracking() override;
  void switchToReferenceModel() override;

  void save() override;
  void printItself(std::ostream &os) const noexcept override;

private:
  bool isSampling() const;

  JsonPositionTracker tracker_;
  int numOfRaysSquared_;
  int numOfVisibleRaysSquared_;
  int currentNumberOfTracking_;
};

// Saves all current collectors arrangement into file.
struct CollectorsTrackerInterface : public Printable {
  virtual ~CollectorsTrackerInterface(){};
  virtual void save(const Collectors &collectors, std::string_view path) = 0;
  void printItself(std::ostream &os) const noexcept override;
};

struct FakeCollectorsTracker : public CollectorsTrackerInterface {
  void save(const Collectors &collectors, std::string_view path) override{};
  void printItself(std::ostream &os) const noexcept override;
};

// saves all collector properies including position and radius, to Json file at
// given path.
struct CollectorsTrackerToJson : public CollectorsTrackerInterface {
  virtual void save(const Collectors &collectors,
                    std::string_view path) override;
  void printItself(std::ostream &os) const noexcept override;
};

} // namespace trackers

#endif