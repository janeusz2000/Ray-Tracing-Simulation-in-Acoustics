#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"

#include <exception>
#include <sstream>
#include <vector>

namespace generators {

class RandomRayOffseter {
public:
  void offsetRay(core::Ray *ray) {
    ray->setDirection(ray->direction() +
                      core::Vec3(getNextAxisOffset(), getNextAxisOffset(), 0));
  }

protected:
  virtual float getNextAxisOffset() = 0;
};

class FakeOffseter : public RandomRayOffseter {
protected:
  float getNextAxisOffset() override { return 0; }
};

class RayFactory {
public:
  virtual bool genRay(core::Ray *ray) = 0;
  virtual core::Vec3 origin() const = 0;
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

  core::Vec3 origin() const override { return origin_; }

private:
  core::Vec3 getDirection(int currentRayIndex) const;
  bool isRayAvailable() const;

  ModelInterface *model_;
  core::Vec3 origin_;
  int numOfRaysAlongEachAxis_;
  int currentRayIndex_;
  float energyPerRay_;
  core::Vec3 targetReferenceDirection_;
};

} // namespace generators

#endif
