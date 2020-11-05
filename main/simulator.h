#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "main/rayTracer.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <vector>

// TODO: radius of each EnergyCOllector should be equal to
// constants::kPi * simulationRadius / numCollectors, to equally cover 3D space
// of between each other. Origin of EnergyCollectors must cross half circle with
// origin at (0, 0, 0) and radius equal to 4 * maximum(model.height or
// model.sideSize) and can't be less then 4 meters - this comes from ISO
// 17497-2:2012: Point source in acoustics must be twice as high as mircophone
// used for measurements where point source is always at (0, 0,
// 8*max(model.sideSize, model.height))
std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const Model &model, int numCollectors);

class Simulator {
public:
  Simulator(RayTracer *tracer, Model *model, generators::RayFactory *source,
            generators::RandomRayOffseter *offsetter)
      : tracer_(tracer), model_(model), source_(source),
        offsetter_(offsetter){};

  std::vector<float> run(float frequency, int numCollectors) {
    std::vector<std::unique_ptr<objects::EnergyCollector>> collectors =
        buildCollectors(*model_, numCollectors);
  }

private:
  RayTracer *tracer_;
  Model *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;
};

#endif