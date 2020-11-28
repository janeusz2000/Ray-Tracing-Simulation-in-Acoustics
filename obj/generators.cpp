#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays, float sourcePower,
                                               ModelInterface *model)
    : numOfRays_(numOfRays), gridPositionMaxValue_(std::sqrt(numOfRays) - 1),
      sourcePower_(sourcePower), model_(model), xGridPosition_(0),
      yGridPosition_(0) {

  checkIfMembersAreValid();
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

void PointSpeakerRayFactory::checkIfMembersAreValid() const {
  if ((gridPositionMaxValue_ + 1) * (gridPositionMaxValue_ + 1) != numOfRays_) {
    throw std::invalid_argument(
        "|numOfRays| must be a  square of another number");
  }
  if (model_->empty()) {
    throw std::invalid_argument("Model Empty!");
  }
}

core::Vec3 PointSpeakerRayFactory::generateDirection() const {
  float u = static_cast<float>(xGridPosition_) / gridPositionMaxValue_ *
            model_->sideSize();
  float v = static_cast<float>(yGridPosition_) / gridPositionMaxValue_ *
            model_->sideSize();
  return gridStart_ + u * core::Vec3::kX + v * core::Vec3::kY - origin_;
}

void PointSpeakerRayFactory::prepareNextDirection() {
  if (xGridPosition_ == gridPositionMaxValue_) {
    xGridPosition_ = 0;
    ++yGridPosition_;
  } else {
    ++xGridPosition_;
  }
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (xGridPosition_ != 0 || yGridPosition_ != gridPositionMaxValue_ + 1);
}
} // namespace generators