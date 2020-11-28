#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays, float sourcePower,
                                               ModelInterface *model)
    : raysPerSideSize_(std::sqrt(numOfRays)), sourcePower_(sourcePower),
      model_(model), xPosition_(0), yPosition_(0) {

  if (raysPerSideSize_ * raysPerSideSize_ != numOfRays) {
    std::stringstream ss;
    ss << "|numOfRays| must be a  square of another number";
    throw std::invalid_argument(ss.str());
  }

  origin_ = 8 * model_->height() * core::Vec3::kZ;
  gridStart_ = -1 * model_->sideSize() / 2 * (core::Vec3::kX + core::Vec3::kY) +
               model_->height() * core::Vec3::kZ;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (!isRayAvailable()) {
    return false;
  }
  *ray = core::Ray(origin_, generateDirection(), sourcePower_);
  prepareNextDirection();
  return true;
}

core::Vec3 PointSpeakerRayFactory::generateDirection() const {
  float u = static_cast<float>(xPosition_) / (raysPerSideSize_ - 1) *
            model_->sideSize();
  float v = static_cast<float>(yPosition_) / (raysPerSideSize_ - 1) *
            model_->sideSize();
  return gridStart_ + u * core::Vec3::kX + v * core::Vec3::kY - origin_;
}

void PointSpeakerRayFactory::prepareNextDirection() {
  std::cout << "xPosition: " << xPosition_ << " yPosition: " << yPosition_
            << std::endl;
  if (xPosition_ == raysPerSideSize_ - 1) {
    xPosition_ = 0;
    yPosition_++;
  } else {
    ++xPosition_;
  }
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (xPosition_ != 0 || yPosition_ != raysPerSideSize_);
}
} // namespace generators