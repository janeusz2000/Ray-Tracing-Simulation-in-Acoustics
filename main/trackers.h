#ifndef TRACKERS_H
#define TRACKERS_H

#include "core/ray.h"
#include "main/model.h"
#include "nlohmann/json.hpp"
#include "obj/objects.h"

#include <fstream>
#include <sstream>
#include <string_view>

namespace trackers {

// Save |model| to the given file as Json file.
void saveModelToJson(std::string_view path, ModelInterface *model);

// Tracks all reached position by rays in the simulation and
// saves them to files in the given path.
class PositionTrackerInterface {
public:
  virtual void initializeNewTracking() = 0;
  virtual void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) = 0;
  virtual void endCurrentTracking() = 0;
  virtual void clearTracking() = 0;
  virtual void save() const = 0;
};

class JsonPositionTracker : public PositionTrackerInterface {
public:
  JsonPositionTracker(std::string_view path);

  // Initialize new tracking of the ray
  void initializeNewTracking() override;
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void endCurrentTracking() override;
  void clearTracking() override;

  void save() const override;

private:
  std::string path_;
  std::vector<core::RayHitData> currentTracking_;
  std::vector<std::vector<core::RayHitData>> trackings_;
};
} // namespace trackers

#endif