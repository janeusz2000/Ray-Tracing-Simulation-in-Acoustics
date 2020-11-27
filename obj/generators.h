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

// generates rays with origin at PointSource origin
// and direction along Z axes down, with offset on XY
// determinated by sampleSize until numOfRay number is reached.
// Note that |numOfRays| must me square of the number.
class PointSpeakerRayFactory : public RayFactory {
public:
  PointSpeakerRayFactory(int numOfRays, ModelInterface *model);

  [[nodiscard]] bool genRay(core::Ray *ray) override;

  core::Vec3 origin() const { return origin_; }

private:
  // Creates Ray based on the grid coordinates, that represent
  // view from the top of the model. For [xPosition, yPosition] 
  // equal to [0, 0], created Ray shoots at lower left
  // corner of the model and when equal
  // to [raysPerSideSize_, raysPerSideSize_] given Ray shoots at
  // upper right corner of the model.
  core::Ray createRay(int xPosition, int yPosition);

  ModelInterface *model_;

  int raysPerSideSize_;
  float simulationSideSize_;

  core::Vec3 origin_;
};

} // namespace generators

#endif
