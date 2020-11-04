#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays, Model *model)
    : numOfRays_(numOfRays), currentRayNum_(0), model_(model) {

  float simulationHeight = 8 * model->height();
  float simulationSideSize = model->sideSize() * 4 / 7;

  origin_ = core::Vec3(0, 0, simulationHeight);
  start_ = -simulationSideSize;
  stop_ = simulationSideSize;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (currentRayNum_ >= numOfRays_) {
    return false;
  }

  return true;
}

} // namespace generators