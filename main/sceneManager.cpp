#include "main/sceneManager.h"
#include <sstream>
SceneManager::SceneManager(int collectors, float simulationRadius)
    : numCollectors_(collectors), simulationRadius_(simulationRadius) {
  if (numCollectors_ % 4 == 0 || (numCollectors_ - 1) % 4 == 0) {
    // ( 2 *  pi * R ) / ( 2 * population)  for dividing whole
    // half circle distance to equal distances for population of
    // energyCollectors
    collectorRadius_ =
        2 * constants::kPi * simulationRadius_ / (2 * numCollectors_);
    createCollectors();
    createReferencePlate();
  } else {
    std::stringstream ss;
    ss << "collector population:" << numCollectors_
       << " % 4 or population - 1: " << numCollectors_ - 1
       << " % 4 must be zero";
    throw std::invalid_argument(ss.str().c_str());
  }
}
std::vector<objects::EnergyCollector *>
SceneManager::getEnergyCollectors() const {
  std::vector<objects::EnergyCollector *> temp;
  temp.reserve(energyCollectors_.size());
  for (const auto &t : energyCollectors_) {
    temp.push_back(t.get());
  }
  return temp;
}

// PRIVATE METHODS

void SceneManager::createCollectors() {
  energyCollectors_.reserve(numCollectors_);
  size_t numToGo;

  if (numCollectors_ % 2 == 0) {
    numToGo = numCollectors_;
  } else {
    energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(
        core::Vec3(0, 0, simulationRadius_ / 2), collectorRadius_));
    numToGo = numCollectors_ - 1;
  }
  std::vector<core::Vec3> origins;

  for (size_t iteration = 0; iteration * 4 < numToGo; ++iteration) {
    float groundCoordinate = simulationRadius_ / 2 *
                             std::cos(2 * constants::kPi * iteration / numToGo);
    float zCoordinate = simulationRadius_ / 2 *
                        std::sin(2 * constants::kPi * iteration / numToGo);

    origins = {
        core::Vec3(groundCoordinate, 0, zCoordinate),
        core::Vec3(0, -groundCoordinate, zCoordinate),
        core::Vec3(-groundCoordinate, 0, zCoordinate),
        core::Vec3(0, groundCoordinate, zCoordinate),
    };

    for (const core::Vec3 &origin : origins) {
      energyCollectors_.push_back(
          std::make_unique<objects::EnergyCollector>(origin, collectorRadius_));
    }
  }
}

void SceneManager::createReferencePlate() {

  if (!referencePlate_.empty()) {
    std::stringstream ss;
    ss << "reference plate not empty: \n";
    for (const auto &tri : referencePlate_) {
      ss << *tri << "\n";
    }
    throw std::logic_error(ss.str());
  }

  std::vector<core::Vec3> points = {
      core::Vec3(simulationRadius_ / 4, simulationRadius_ / 4, 0),
      core::Vec3(simulationRadius_ / 4, -simulationRadius_ / 4, 0),
      core::Vec3(-simulationRadius_ / 4, simulationRadius_ / 4, 0),
      core::Vec3(-simulationRadius_ / 4, -simulationRadius_ / 4, 0)};

  referencePlate_.reserve(2);
  referencePlate_.push_back(
      std::make_unique<objects::TriangleObj>(points[0], points[1], points[2]));
  referencePlate_.push_back(
      std::make_unique<objects::TriangleObj>(points[1], points[2], points[3]));
}