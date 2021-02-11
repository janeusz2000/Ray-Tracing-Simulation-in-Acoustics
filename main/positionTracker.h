#ifndef POSITION_TRACKER_H
#define POSITION_TRACKER_H

#include "core/ray.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <string_view>


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

#endif