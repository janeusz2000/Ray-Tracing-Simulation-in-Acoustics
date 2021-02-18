#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/positionTracker.h"
#include "main/rayTracer.h"
#include "nlohmann/json.hpp"
#include "obj/generators.h"
#include "obj/objects.h"

#include <cmath>
#include <fstream>
#include <future>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <vector>

// Constructs an array of Energy Collectors around specified model.
// Energy Collectors are arranged in two half-circles, whose origin is
// centere on the model, oriented at the right angle to each other.
// Their radius is equal to 4 * max(model height, model side),
// but not less then 4.

// Collectors are arranged uniformly on the circumference's, such that bottom
// collectors are on the ground level.
// In case of odd number of collectors, one collector is placed right above the
// model. Radius of an energy collector is equal to distance between twoenergy
// collectors.

// Throws std::invalid_argument when |numCollectors| < 4 or when |numCollectors|
// or |numCollectors|-1 is not divisible by 4.
std::vector<objects::EnergyCollector>
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
  void run(float frequency, std::vector<objects::EnergyCollector> &collectors,
           std::promise<std::vector<float>> *promise);

private:
  std::vector<float> getEnergyFromGivenCollectors(
      const std::vector<objects::EnergyCollector> &collectors);

  RayTracer *tracer_;
  ModelInterface *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;

  trackers::PositionTracker *positionTracker_;
  collectionRules::CollectEnergyInterface *energyCollectionRules_;
};

#endif