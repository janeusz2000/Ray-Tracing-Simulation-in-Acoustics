#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRaysAlongEachAxis,
                                               float sourcePower,
                                               ModelInterface *model)
    : model_(model), numOfRaysAlongEachAxis_(numOfRaysAlongEachAxis),
      currentRayIndex_(0),
      energyPerRay_(sourcePower /
                    (numOfRaysAlongEachAxis * numOfRaysAlongEachAxis)) {

  if (numOfRaysAlongEachAxis_ <= 0) {
    std::stringstream ss;
    ss << "|numOfRaysAlongEachAxis| cannot be equal or less than zero! "
          "\n|numOfRaysAlongEachAxis|: "
       << numOfRaysAlongEachAxis_;
    throw std::invalid_argument(ss.str());
  }
  if (sourcePower < 0) {
    std::stringstream ss;
    ss << "|sourcePower| power cannot be less than zero! \n|sourcePower|: "
       << sourcePower;
    throw std::invalid_argument(ss.str());
  }
  if (model_->empty()) {
    throw std::invalid_argument("Model cannot be Empty!");
  }

  // Comes from the requirements of the ISO 17497-2:2012, which says that point
  // source must be placed at least twice as high as the microphone radius
  // array. Because microphone radius array is equal to:
  // |kSimulationHeight| / 2 * model.height(), we can assume:
  origin_ =
      core::Vec3(0, 0,
                 std::max(static_cast<float>(constants::kSimulationHeight),
                          constants::kSimulationHeight * model_->height()));

  float sizeFactor = -model_->sideSize();
  targetReferenceDirection_ =
      core::Vec3(sizeFactor, sizeFactor, model->height()) - origin_;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (!isRayAvailable()) {
    return false;
  }
  *ray = core::Ray(origin_, getDirection(currentRayIndex_), energyPerRay_);
  ++currentRayIndex_;
  return true;
}
core::Vec3 PointSpeakerRayFactory::getDirection(int currentRayIndex) const {
  if (numOfRaysAlongEachAxis_ == 1) {
    return -core::Vec3::kZ;
  }

  int xIndex = currentRayIndex % numOfRaysAlongEachAxis_;
  int yIndex = currentRayIndex / numOfRaysAlongEachAxis_;

  float u = 2 * static_cast<float>(xIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();
  float v = 2 * static_cast<float>(yIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();

  return targetReferenceDirection_ + core::Vec3(u, v, 0);
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (currentRayIndex_ < numOfRaysAlongEachAxis_ * numOfRaysAlongEachAxis_);
}
} // namespace generators

void collectionRules::LinearEnergyCollection::collectEnergy(
    std::vector<objects::EnergyCollector> &collectors,
    core::RayHitData *hitData) {

  core::Vec3 reachedPosition = hitData->collisionPoint();
  for (auto &energyCollector : collectors) {
    if (energyCollector.isVecInside(reachedPosition)) {
      float distanceToOrigin =
          (energyCollector.getOrigin() - reachedPosition).magnitude();

      // The closer ray hits origin of the energy Collector, the more energy
      // energyCollector collects.
      float energyRatio = 1 - distanceToOrigin / energyCollector.getRadius();
      energyCollector.addEnergy(energyRatio * hitData->energy());
    };
  }
}