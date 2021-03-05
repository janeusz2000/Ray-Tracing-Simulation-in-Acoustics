#ifndef TRACKERS_H
#define TRACKERS_H

#include "core/classUtlilities.h"
#include "core/ray.h"
#include "main/model.h"
#include "nlohmann/json.hpp"
#include "obj/objects.h"

#include <fstream>
#include <sstream>
#include <string_view>

// Contains objects and functions that are responsible for exporting calculated
// data, objects and ray trajectories in simulation to different files. They
// are used for visualization and debugging purposes.
namespace trackers {

using Json = nlohmann::json;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

// Mediator between different type of input data File.
// Prepares data to be saved into file
struct FileBuffer {
  void addMessageToBuffer(std::string_view message);
  // clears buffer and acquires data from json to buffer`
  void acquireJsonFile(const Json &json);
  std::stringstream stream;
};

// Mediator between std::ofstream and FileBuffer.
// Manages opening, saving, writing and closing file.
class FileInterface : public Printable {
public:
  // TODO: And check if file is open
  virtual void openFileWithOverwrite() = 0;
  // TODO: And check if file is open
  virtual void open() = 0;
  virtual void write(const FileBuffer &buffer) = 0;
  virtual void writeWithoutFlush(const FileBuffer &buffer) = 0;
  void printItself(std::ostream &os) const noexcept override;
};

// Mediator between std::ofstream and FileBuffer.
// Manages opening, saving, writing and closing file.
class File : public FileInterface {
public:
  explicit File(std::string_view path) : path_(path){};
  // Opens file at given |path| overwriting existing one.
  // Throws std::invalid_argument exception if file is not found
  // at given path.
  void openFileWithOverwrite();
  // Opens file at given |path| without overwriting it.
  // Data is appended after existing one. Throws
  // std::invalid_argument exception if file is not found
  // at given path.
  void open();
  // Writes given FileBuffer into file and flush content;
  void write(const FileBuffer &buffer);
  void writeWithoutFlush(const FileBuffer &buffer);
  void printItself(std::ostream &os) const noexcept override;

private:
  void handleErrors();

protected:
  std::ofstream fileStream_;
  std::string_view path_;
};

// contains utilities for rendering javascript syntax in FileBuffer
namespace javascript {

// TODO: create constructor of the FleBuffer file with json.

FileBuffer initVar(std::string_view variableName);
FileBuffer initLet(std::string_view variableName);
FileBuffer initConst(std::string_view variableName);
FileBuffer initArray();
FileBuffer endArray();
FileBuffer initObject();
FileBuffer endObject();

void endLineInBuffer(FileBuffer &buffer);
void initArrayInBuffer(FileBuffer &buffer);
void endArrayInBuffer(FileBuffer &buffer);
void initObjectInBuffer(FileBuffer &buffer);
void endObjectInBuffer(FileBuffer &buffer);
void initConstInBuffer(FileBuffer &buffer, std::string_view constName);
} // namespace javascript

// Represent collected energy value from each collector
// at Collectors at the same index.
using Energies = std::vector<float>;
// Represent collected energy from collectors at the given float that
// represent frequency of the simulation.
using EnergyPerFrequency = std::unordered_map<float, Energies>;

// Save results of the simulation at given |path| as results.js file, with json
// structure. Change |referenceModel|
void saveResultsAsJson(std::string_view path, const EnergyPerFrequency &results,
                       bool referenceModel = false);

// Save |model| to the given file as Json file.
void saveModelToJson(std::string_view path, ModelInterface *model);

// Tracks all reached position by rays in the simulation and
// saves them to files in the given path.
class PositionTrackerInterface : public Printable {
public:
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

// Tracks all reached rays position and saves them to js file as json data at
// given path. REQUIREMENTS: file must exist at given path.
class JsonPositionTracker : public PositionTrackerInterface {
public:
  JsonPositionTracker(std::string path);

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
  virtual void save(const Collectors &collectors, std::string_view path) = 0;
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