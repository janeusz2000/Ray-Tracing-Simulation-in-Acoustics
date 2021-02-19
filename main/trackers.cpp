#include "trackers.h"

namespace trackers {
void saveModelToJson(std::string_view pathToFolder, ModelInterface *model) {

  if (model == nullptr) {
    throw std::invalid_argument("Model cannot be nullptr! ");
  }

  std::string outputPath = pathToFolder.data();
  outputPath += "/model.json";
  std::ofstream outFile(outputPath);
  if (!outFile.good()) {
    std::stringstream ss;
    ss << "Invalid path \nGiven Path: " << outputPath << std::endl;
    throw std::invalid_argument(ss.str());
  }

  using Json = nlohmann::json;
  Json outputJson = Json::array();

  for (const objects::TriangleObj &triangle : model->triangles()) {
    Json currentTriangle = {{"point1",
                             {{"x", triangle.point1().x()},
                              {"y", triangle.point1().y()},
                              {"z", triangle.point1().z()}}},
                            {"point2",
                             {{"x", triangle.point2().x()},
                              {"y", triangle.point2().y()},
                              {"z", triangle.point2().z()}}},
                            {"point3",
                             {{"x", triangle.point3().x()},
                              {"y", triangle.point3().y()},
                              {"z", triangle.point3().z()}}}};
    outputJson.push_back(currentTriangle);
  }

  outFile << outputJson.dump(2);
  outFile.close();
}

JsonPositionTracker::JsonPositionTracker(std::string_view path)
    : path_(path.data()){};

void JsonPositionTracker::clearTracking() {
  currentTracking_.clear();
  trackings_.clear();
}
void JsonPositionTracker::initializeNewTracking() { currentTracking_.clear(); }
void JsonPositionTracker::addNewPositionToCurrentTracking(
    const core::RayHitData &hitData) {
  currentTracking_.push_back(hitData);
}
void JsonPositionTracker::endCurrentTracking() {
  trackings_.push_back(currentTracking_);
}

void JsonPositionTracker::save() const {
  using Json = nlohmann::json;

  Json outputJson = Json::array();
  for (const auto &tracking : trackings_) {
    Json trackingJson = Json::array();
    bool first = true;
    for (const core::RayHitData &hitData : tracking) {
      if (first) { // ! TODO: this is bad. Fix it! 3;)
        first = false;
        continue;
      }
      Json hitDataJson = {
          {"origin",
           {{"x", hitData.origin().x()},
            {"y", hitData.origin().y()},
            {"z", hitData.origin().z()}}},
          {"direction",
           {
               {"x", hitData.direction().x()},
               {"y", hitData.direction().y()},
               {"z", hitData.direction().z()},
           }},
          {"energy", hitData.energy()},
          {"length",
           (hitData.origin() - hitData.collisionPoint()).magnitude()}};
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
} // namespace trackers