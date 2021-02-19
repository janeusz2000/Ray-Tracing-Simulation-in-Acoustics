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

namespace collectionRules {

// defines how energy collectors collect energy in the simulation
struct CollectEnergyInterface {
  virtual void collectEnergy(
      const std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors,
      core::RayHitData *hitData) = 0;
};

// The futher away from origin of energy collectors ray hits, the less energy it
// puts in the energy collector - left energy scales lineary. When distanece
// betwen hit position and energyCollector origin is equal or bigger then radius
// of the energyCollector, none energy is put inside the energy Collector. Phase
// impact of the wave is not considered here.
struct LinearEnergyCollection : public CollectEnergyInterface {
  void collectEnergy(
      const std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors,
      core::RayHitData *hitData) override;
};

// TODO: Create Rules for no linear collection
// TODO: Create Rules for phase impact of the simulation
// TODO: Create Combined Rules of collection
} // namespace collectionRules

#endif
