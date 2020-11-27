#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays,
                                               ModelInterface *model)
    : model_(model), currentRayNumX_(0), currentRayNumY_(-1) {

  int numOfRaysPerModelSideSize_ = std::sqrt(numOfRays);
  if (numOfRaysPerModelSideSize_ * numOfRaysPerModelSideSize_ != numOfRays) {
    std::stringstream ss;
    ss << "|numOfRays| must be square of another number";
    throw std::invalid_argument(ss.str());
  }

  simulationHeight_ = 8 * model->height();

  origin_ = core::Vec3(0, 0, simulationHeight_);
  start_ = model->height() * core::Vec3::kZ -
           model->sideSize() / 2 * core::Vec3::kX -
           model->sideSize() / 2 * core::Vec3::kY;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  // if PointSpeakerRayFactory produced all rays:
  if (currentRayNumY_ > numOfRaysPerModelSideSize_) {
    return false;
  }

  if (currentRayNumX_ % numOfRaysPerModelSideSize_ == 0) {
    currentRayNumX_ = 0;
    ++currentRayNumY_;
  }
  core::Vec3 direction =
      start_ +
      static_cast<float>(currentRayNumX_) /
          (numOfRaysPerModelSideSize_)*model_->sideSize() * core::Vec3::kX +
      static_cast<float>(currentRayNumY_) /
          (numOfRaysPerModelSideSize_)*model_->sideSize() * core::Vec3::kY -
      origin_;

  ++currentRayNumX_;
  *ray = core::Ray(origin_, direction);
  return true;
}

} // namespace generators