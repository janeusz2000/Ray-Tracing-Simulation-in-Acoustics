#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"

#include <exception>
#include <utility>

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

// Generates rays with origin at PointSource origin
// and direction along Z axes down, with offset on XY
// determinated by sampleSize until numOfRay number is reached.
// |numOfRays| must me square of the number.
// |sourcePower| determine how much energy each ray has.
// TODO: change impelmentation that match representation which says, ray has
// TODO: energy in defined in [J] when source power is defined in [W]
class PointSpeakerRayFactory : public RayFactory {
public:
  PointSpeakerRayFactory(int numOfRays, float sourcePower,
                         ModelInterface *model);

  [[nodiscard]] bool genRay(core::Ray *ray) override;

  core::Vec3 origin() const { return origin_; }

private:
  core::Vec3 generateDirection() const;
  bool isRayAvailable() const;
  void prepareNextDirection();

  ModelInterface *model_;
  core::Vec3 origin_;
  // starting point of the grid that represents lower left corner of view from
  // the top of the model
  core::Vec3 gridStart_;
  int xPosition_, yPosition_, raysPerSideSize_;
  float sourcePower_;
};

} // namespace generators

#endif
