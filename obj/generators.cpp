#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRaysAlongEachAxis,
                                               float sourcePower,
                                               ModelInterface *model)
    : numOfRaysAlongEachAxis_(numOfRaysAlongEachAxis),
      energyPerRay_(sourcePower /
                    (numOfRaysAlongEachAxis * numOfRaysAlongEachAxis)),
      model_(model), currentRayIndex_(0) {

  if (numOfRaysAlongEachAxis_ <= 0) {
    std::stringstream ss;
    ss << "|numOfRaysAlongEachAxis| cannot be equal or less then zero! "
          "\n|numOfRaysAlongEachAxis|: "
       << numOfRaysAlongEachAxis_;
    throw std::invalid_argument(ss.str());
  }
  if (sourcePower < 0) {
    std::stringstream ss;
    ss << "|sourcePower| power cannot be less then zero! \n|sourcePower|: "
       << sourcePower;
    throw std::invalid_argument(ss.str());
  }
  if (model_->empty()) {
    throw std::invalid_argument("Model cannot be Empty!");
  }

  using core::Vec3;
  // Comes from the requirements of the ISO 17497-2:2012, which says that point
  // source must be placed at least twice as high as the microphone radius
  // array. Because microphone radius array is equal to:
  // |kSimulationHeight| / 2 * model.height(), we can assume:
  origin_ = Vec3(0, 0, constants::kSimulationHeight * model_->height());

  float sizeFactor = -0.5 * model_->sideSize();
  targetReferenceDirection_ =
      Vec3(sizeFactor, sizeFactor, model->height()) - origin_;
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
  int xIndex = currentRayIndex % numOfRaysAlongEachAxis_;
  int yIndex = currentRayIndex / numOfRaysAlongEachAxis_;

  float u = static_cast<float>(xIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();
  float v = static_cast<float>(yIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();

  return targetReferenceDirection_ + core::Vec3(u, v, 0);
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (currentRayIndex_ < numOfRaysAlongEachAxis_ * numOfRaysAlongEachAxis_);
}
} // namespace generators