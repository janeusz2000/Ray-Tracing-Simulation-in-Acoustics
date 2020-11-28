#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"

#include <exception>
#include <sstream>

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
  virtual bool genRay(core::Ray *ray) = 0;
};

// Generates rays with origin at PointSource origin
// and direction along Z axes down, with offset on XY
// determinated by sampleSize until numOfRay number is reached.
// |numOfRays| must me square of the number.
// |sourcePower| represents power of the source in [W]
class PointSpeakerRayFactory : public RayFactory {
public:
  PointSpeakerRayFactory(int numOfRays, float sourcePower,
                         ModelInterface *model);

  [[nodiscard]] bool genRay(core::Ray *ray) override;

  core::Vec3 origin() const { return origin_; }

private:
  core::Vec3 generateDirection() const;
  bool isRayAvailable() const;
  void checkIfMembersAreValid() const;
  void prepareNextDirection();

  ModelInterface *model_;
  core::Vec3 origin_;
  int numOfRays_;
  float sourcePower_;
  // Parameters describe the mesh created from the top view of the model.
  // [xMeshPosition, yMeshPosition] equal to [0, 0] represents lower left corner
  // of the mesh, whereas equal to [meshPositionMaxValue, meshPositionMaxValue]
  // represents upper right corner of the mesh.
  core::Vec3 meshStart_;
  int xMeshPosition_, yMeshPosition_, meshPositionMaxValue_;
};

} // namespace generators

#endif
