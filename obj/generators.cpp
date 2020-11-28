#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays, float sourcePower,
                                               ModelInterface *model)
    : numOfRays_(numOfRays), meshPositionMaxValue_(std::sqrt(numOfRays) - 1),
      sourcePower_(sourcePower), model_(model), xMeshPosition_(0),
      yMeshPosition_(0) {

  checkIfMembersAreValid();
  origin_ = 8 * model_->height() * core::Vec3::kZ;
  meshStart_ = -1 * model_->sideSize() / 2 * (core::Vec3::kX + core::Vec3::kY) +
               model_->height() * core::Vec3::kZ;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (!isRayAvailable()) {
    return false;
  }
  *ray = core::Ray(origin_, generateDirection(), sourcePower_ / numOfRays_);
  prepareNextDirection();
  return true;
}

void PointSpeakerRayFactory::checkIfMembersAreValid() const {
  if (numOfRays_ <= 0) {
    std::stringstream ss;
    ss << "|numOfRays| cannot be equal or less then zero! \n|numOfRays|: "
       << numOfRays_;
    throw std::invalid_argument(ss.str());
  }
  if ((meshPositionMaxValue_ + 1) * (meshPositionMaxValue_ + 1) != numOfRays_) {
    throw std::invalid_argument(
        "|numOfRays| must be a  square of another number");
  }
  if (model_->empty()) {
    throw std::invalid_argument("Model Empty!");
  }
}

core::Vec3 PointSpeakerRayFactory::generateDirection() const {
  float u = static_cast<float>(xMeshPosition_) / meshPositionMaxValue_ *
            model_->sideSize();
  float v = static_cast<float>(yMeshPosition_) / meshPositionMaxValue_ *
            model_->sideSize();
  return meshStart_ + u * core::Vec3::kX + v * core::Vec3::kY - origin_;
}

void PointSpeakerRayFactory::prepareNextDirection() {
  if (xMeshPosition_ == meshPositionMaxValue_) {
    xMeshPosition_ = 0;
    ++yMeshPosition_;
  } else {
    ++xMeshPosition_;
  }
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (xMeshPosition_ != 0 || yMeshPosition_ != meshPositionMaxValue_ + 1);
}
} // namespace generators