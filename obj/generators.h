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

// Generates rays aimed at given model from predetermined |origin| of the
// PointSpeakerRayFactory with genRay() method until number of generated Rays
// equal to |numOfRaysAlongEachAxis|^2 is reached.
class PointSpeakerRayFactory : public RayFactory {
public:
  // |numOfRaysAlongEachAxis| must be greater than 0,
  // |sourcePower| cannot be less then 0,
  //               represents power of the source in [W],
  // |model| must not be empty.
  PointSpeakerRayFactory(int numOfRaysAlongEachAxis, float sourcePower,
                         ModelInterface *model);

  [[nodiscard]] bool genRay(core::Ray *ray) override;

  core::Vec3 origin() const { return origin_; }

private:
  // TODO: explain what rayIndexAtXAxis and rayIndexAtYAxis is.
  core::Vec3 getDirection(int rayIndexAtXAxis, int rayIndexAtYAxis) const;
  bool isRayAvailable() const;
  // Checks if:
  // |numOfRaysAlongEachAxis| is equal or less then 0,
  // |sourcePower| less then 0,
  // |model| is empty.
  void checkIfClassMembersAreValid() const;
  void updateCurrentRayIndexes();

  ModelInterface *model_;
  core::Vec3 origin_;
  int numOfRaysAlongEachAxis_;
  float sourcePower_;
  // TODO: explain this better because this is really bad.
  core::Vec3 targetReferenceDirection_;
  int currentRayIndexAlongXAxis_, currentRayIndexAlongYAxis_;
};

} // namespace generators

#endif
