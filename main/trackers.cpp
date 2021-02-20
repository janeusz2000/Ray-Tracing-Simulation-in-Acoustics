#include "trackers.h"

namespace trackers {
void saveModelToJson(std::string_view pathToFolder, ModelInterface *model) {

  if (model == nullptr) {
    throw std::invalid_argument(
        "Model in saveModelToJson() cannot be nullptr! ");
  }

  std::string outputPath = pathToFolder.data();
  outputPath += "/model.json";
  std::ofstream outFile(outputPath);
  if (!outFile.good()) {
    std::stringstream ss;
    ss << "Invalid path given to saveToModel()!\n"
       << "Given Path: " << outputPath << "\n"
       << "file doesn't exist?";
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

std::ostream &operator<<(std::ostream &os,
                         const PositionTrackerInterface &tracker) {
  tracker.printItself(os);
  return os;
}

JsonPositionTracker::JsonPositionTracker(std::string_view path)
    : path_(path.data()){};

void JsonPositionTracker::printItself(std::ostream &os) const noexcept {
  os << "Json Position Tracker\n"
     << "Path: " << path_.data() << "\n"
     << "current tracking: \n";
  int currentHitData = 0;
  for (const core::RayHitData &hitData : currentTracking_) {
    os << "HitData number " << currentHitData << "\n" << hitData << "\n";
  }
  os << "Trackings overall size: " << trackings_.size();
}

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
    ss << "Invalid path given to: \n"
       << *this << "make sure that file at path: \""
       << path_ + "/trackingData.json\""
       << " exist";

    throw std::invalid_argument(ss.str());
  }
  outFile << outputJson.dump(2);
  outFile.close();
}

std::ostream &operator<<(std::ostream &os,
                         const CollectorsTrackerInterface &tracker) {
  tracker.printItself(os);
  return os;
}

// exports |energyCollectors| as string representation to |path|
void CollectorsTrackerToJson::save(const Collectors &energyCollectors,
                                   std::string_view path) {

  std::string resultPath = path.data();
  resultPath += "/energyCollectors.json";
  std::ofstream outFile(resultPath);
  if (!outFile.good()) {
    std::stringstream errorStream;
    errorStream << "File at given path to: " << *this
                << "check if file at: " << resultPath << "exist!";
    throw std::invalid_argument(errorStream.str());
  }

  using Json = nlohmann::json;
  Json outArray = Json::array();
  int currentCollectorNumber = 0;
  for (const auto &collector : energyCollectors) {
    core::Vec3 collectorOrigin = collector->getOrigin();
    float radius = collector->getRadius();
    Json energyCollector = {{"number", currentCollectorNumber},
                            {"x", collectorOrigin.x()},
                            {"y", collectorOrigin.y()},
                            {"z", collectorOrigin.z()},
                            {"radius", radius}};
    outArray.push_back(energyCollector);
    ++currentCollectorNumber;
  }

  outFile << outArray.dump(1);
  outFile.close();
}

void CollectorsTrackerToJson::printItself(std::ostream &os) const noexcept {
  os << "Json Collectors Tracker \n";
}
} // namespace trackers