#include "positionTracker.h"

PositionTracker::PositionTracker(std::string_view path) : path_(path.data()){};

void PositionTracker::clearTracking() {
  currentTracking_.clear();
  trackings_.clear();
}
void PositionTracker::initializeNewTracking() { currentTracking_.clear(); }
void PositionTracker::addNewPositionToCurrentTracking(
    const core::RayHitData &hitData) {
  currentTracking_.push_back(hitData);
}
void PositionTracker::endCurrentTracking() {
  trackings_.push_back(currentTracking_);
}

void PositionTracker::saveAsJson() const {
  using Json = nlohmann::json;

  Json outputJson = Json::array();
  for (const auto &tracking : trackings_) {
    Json trackingJson = Json::array();
    for (const core::RayHitData &hitData : tracking) {
      Json hitDataJson = {{"start",
                           {{"x", hitData.origin().x()},
                            {"y", hitData.origin().y()},
                            {"z", hitData.origin().z()},
                            {"energy", hitData.energy()}},
                           "end",
                           {{"x", hitData.collisionPoint().x()},
                            {"y", hitData.collisionPoint().y()},
                            {"z", hitData.collisionPoint().z()},
                            {"energy", hitData.energy()}}}};
      trackingJson.push_back(hitDataJson);
    }
    outputJson.push_back(trackingJson);
  }
  std::ofstream outFile(path_ + "/trackingData.json", std::ios_base::trunc);
  if (!outFile.good()) {
    std::stringstream ss;
    ss << "Invalid path to the object!"
       << "\n"
       << "path: " << path_ + "/trackingData.json";

    throw std::invalid_argument(ss.str());
  }
  outFile << outputJson.dump(2);
  outFile.close();
}