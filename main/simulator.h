#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/positionTracker.h"
#include "main/rayTracer.h"
#include "nlohmann/json.hpp"
#include "obj/generators.h"
#include "obj/objects.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <vector>

namespace collectionRules {

// defines how energy collectors collect energy in the simulation
struct CollectEnergyInterface {
  virtual void collectEnergy(
      std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors,
      core::RayHitData *hitData) = 0;
};

// The futher away from origin of energy collectors ray hits, the less energy it
// puts in the energy collector - left energy scales lineary. When distanece
// betwen hit position and energyCollector origin is equal or bigger then radius
// of the energyCollector, none energy is put inside the energy Collector. Phase
// impact of the wave is not considered here.
struct LinearEnergyCollection : public CollectEnergyInterface {
  void collectEnergy(
      std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors,
      core::RayHitData *hitData) override;
};

// TODO: Create Rules for no linear collection
// TODO: Create Rules for phase impact of the simulation
// TODO: Create Combined Rules of collection
} // namespace collectionRules

// Constructs an array of Energy Collectors around specified model.
// Energy Collectors are arranged in two half-circles, whose origin is
// centere on the model, oriented at the right angle to each other.
// Their radius is equal to 4 * max(model height, model side),
// but not less then 4.

// Collectors are arranged uniformly on the circumference's, such that bottom
// collectors are on the ground level.
// In case of odd number of collectors, one collector is placed right above the
// model. Radius of an energy collector is equal to distance between two
// collectors.

// Throws std::invalid_argument when |numCollectors| < 4 or when |numCollectors|
// or |numCollectors|-1 is not divisible by 4.
std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const ModelInterface *model, int numCollectors);

// Saves positions of the energyCollectors to the Json file at given path.
void exportCollectorsToJson(
    const std::vector<std::unique_ptr<objects::EnergyCollector>>
        &energyCollectors,
    std::string_view path);

// Calculates maximum radius of the sphere that limits size of the
// simulation Rays cannot reach any position outside of the SphereWall
// object.
const float getSphereWallRadius(const ModelInterface &model);

// Performs ray-tracing simulation on given model.
class Simulator {
public:
  Simulator(RayTracer *tracer, ModelInterface *model,
            generators::RayFactory *source,
            generators::RandomRayOffseter *offsetter,
            trackers::PositionTracker *positionTracker,
            collectionRules::CollectEnergyInterface *energyCollectionRules)
      : tracer_(tracer), model_(model), source_(source), offsetter_(offsetter),
        positionTracker_(positionTracker),
        energyCollectionRules_(energyCollectionRules){};

  // Runs the simulation and returns vector of float that represent result
  // energy collected by energyCollectors. Index of the float correspond
  // with index of builded energy collector.
  std::vector<float>
  run(float frequency,
      std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors);

private:
  std::vector<float> getEnergyFromGivenCollectors(
      const std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors);

  RayTracer *tracer_;
  ModelInterface *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;

  trackers::PositionTracker *positionTracker_;
  collectionRules::CollectEnergyInterface *energyCollectionRules_;
};

#endif