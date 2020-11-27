#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"

namespace generators {

struct RandomRayOffseter {
public:
  core::Ray offsetRay(const core::Ray &ray) {
    return core::Ray(ray.origin(),
                     ray.direction() + core::Vec3(getNextAxisOffset(),
                                                  getNextAxisOffset(), 0));
  }

protected:
  virtual float getNextAxisOffset() const = 0;
};

class RayFactory {
public:
  // generates ray with properties determinated by each class
  // example: Speaker source is a membrane that is generating
  // rays that have origins where membrane is situated and direction
  // parpendicular to surface where membrane is situated.
  virtual bool genRay(core::Ray *ray) = 0;
};

// generates rays with origin at PointSource origin
// and direction along Z axes down, with offset on XY
// determinated by sampleSize until numOfRay number is reached
class PointSpeakerRayFactory : public RayFactory {
public:
  PointSpeakerRayFactory(int numOfRays, ModelInterface *model);

  [[nodiscard]] bool genRay(core::Ray *ray) override;

  core::Vec3 origin() const { return origin_; }

private:
  ModelInterface *model_;

  int numOfRaysPerModelSideSize_, currentRayNumX_, currentRayNumY_;
  float simulationHeight_, simulationSideSize_;
  core::Vec3 origin_, start_;
};

} // namespace generators

#endif
