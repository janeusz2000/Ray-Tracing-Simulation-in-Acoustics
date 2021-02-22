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

using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

namespace trackers {

// Save |model| to the given file as Json file.
void saveModelToJson(std::string_view path, ModelInterface *model);

// Tracks all reached position by rays in the simulation and
// saves them to files in the given path.
class PositionTrackerInterface : public Printable {
public:
  virtual void initializeNewTracking() = 0;
  virtual void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) = 0;
  virtual void endCurrentTracking() = 0;
  virtual void save() = 0;

  void printItself(std::ostream &os) const noexcept override;
};

// Tracks all reached rays position and saves them to js file as json data at
// given path. REQUIREMENTS: file must exist at given path.
class JsonPositionTracker : public PositionTrackerInterface {
public:
  JsonPositionTracker(std::string_view path);

  // Initialize new tracking of the ray
  void initializeNewTracking() override;
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void endCurrentTracking() override;

  void save() override;
  void printItself(std::ostream &os) const noexcept override;

private:
  std::ofstream outFile_;
  std::string path_;
  std::vector<core::RayHitData> currentTracking_;
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

  virtual void initializeNewTracking() override;
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void endCurrentTracking() override;

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