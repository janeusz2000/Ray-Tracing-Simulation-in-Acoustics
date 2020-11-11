#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays,
                                               ModelInterface *model)
    : numOfRays_(numOfRays), currentRayNum_(0) {

  simulationHeight_ = 8 * model->height();
  simulationSideSize_ = model->sideSize() * 4 / 7;

  origin_ = core::Vec3(0, 0, simulationHeight_);
  start_ = -simulationSideSize_;
  stop_ = simulationSideSize_;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (currentRayNum_ >= numOfRays_) {
    return false;
  }

  return true;
}

} // namespace generators