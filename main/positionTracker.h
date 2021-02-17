#ifndef POSITION_TRACKER_H
#define POSITION_TRACKER_H

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
class PositionTracker {
public:
  PositionTracker(std::string_view path);

  // Initialize new tracking of the ray
  void initializeNewTracking();
  void addNewPositionToCurrentTracking(const core::RayHitData &hitData);
  void endCurrentTracking();
  void clearTracking();

  void saveAsJson() const;

private:
  std::string path_;
  std::vector<core::RayHitData> currentTracking_;
  std::vector<std::vector<core::RayHitData>> trackings_;
};
} // namespace trackers

#endif