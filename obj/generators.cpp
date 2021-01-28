#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRaysAlongEachAxis,
                                               float sourcePower,
                                               ModelInterface *model)
    : numOfRaysAlongEachAxis_(numOfRaysAlongEachAxis),
      sourcePower_(sourcePower), model_(model), currentRayIndexAlongXAxis_(0),
      currentRayIndexAlongYAxis_(0) {

  checkIfClassMembersAreValid();

  // Comes from the requirements of the ISO 17497-2:2012, which says that point
  // source must be placed at least twice as high as the microphone radius
  // array. Because microphone radius array is equal to:
  // |kSimulationHeight| / 2 * model.height(), we can assume:
  origin_ = constants::kSimulationHeight * model_->height() * core::Vec3::kZ;

  using core::Vec3;
  targetReferenceDirection_ =
      -1 * model_->sideSize() / 2 * (Vec3::kX + Vec3::kY) +
      model_->height() * Vec3::kZ - origin_;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (!isRayAvailable()) {
    return false;
  }
  *ray = core::Ray(
      origin_,
      getDirection(currentRayIndexAlongXAxis_, currentRayIndexAlongYAxis_),
      sourcePower_ / (numOfRaysAlongEachAxis_ * numOfRaysAlongEachAxis_));
  updateCurrentRayIndexes();
  return true;
}

void PointSpeakerRayFactory::checkIfClassMembersAreValid() const {
  if (numOfRaysAlongEachAxis_ <= 0) {
    std::stringstream ss;
    ss << "|numOfRaysAlongEachAxis| cannot be equal or less then zero! "
          "\n|numOfRaysAlongEachAxis|: "
       << numOfRaysAlongEachAxis_;
    throw std::invalid_argument(ss.str());
  }
  if (sourcePower_ < 0) {
    std::stringstream ss;
    ss << "|sourcePower| power cannot be less then zero! \n|sourcePower|: "
       << sourcePower_;
    throw std::invalid_argument(ss.str());
  }

  if (model_->empty()) {
    throw std::invalid_argument("Model cannot be Empty!");
  }
}

core::Vec3 PointSpeakerRayFactory::getDirection(int rayIndexAtXAxis,
                                                int rayIndexAtYAxis) const {
  float u = static_cast<float>(rayIndexAtXAxis) /
            (numOfRaysAlongEachAxis_ - 1) * model_->sideSize();
  float v = static_cast<float>(rayIndexAtYAxis) /
            (numOfRaysAlongEachAxis_ - 1) * model_->sideSize();
  return targetReferenceDirection_ + u * core::Vec3::kX + v * core::Vec3::kY;
}

void PointSpeakerRayFactory::updateCurrentRayIndexes() {
  if (currentRayIndexAlongXAxis_ == numOfRaysAlongEachAxis_ - 1) {
    currentRayIndexAlongXAxis_ = 0;
    ++currentRayIndexAlongYAxis_;
  } else {
    ++currentRayIndexAlongXAxis_;
  }
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (currentRayIndexAlongXAxis_ != 0 ||
          currentRayIndexAlongYAxis_ != numOfRaysAlongEachAxis_);
}
} // namespace generators