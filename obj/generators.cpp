#include "generators.h"

namespace generators {

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRays,
                                               ModelInterface *model)
    : raysPerSideSize_(std::sqrt(numOfRays)), model_(model) {

  if (raysPerSideSize_ * raysPerSideSize_ != numOfRays) {
    std::stringstream ss;
    ss << "|numOfRays| must be a  square of another number";
    throw std::invalid_argument(ss.str());
  }

  origin_ = core::Vec3(0, 0, 8 * model->height());
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) { return true; }

core::Ray PointSpeakerRayFactory::createRay(int xPosition, int yPosition) {

  // TODO: change this for the instruction described in comment

  core::Vec3 direction =
      model_->height() * core::Vec3::kZ +
      static_cast<float>(xPosition) / raysPerSideSize_ * core::Vec3::kX +
      static_cast<float>(yPosition) / raysPerSideSize_ * core::Vec3::kY -
      origin_;
  return core::Ray(origin_, direction);
}

} // namespace generators