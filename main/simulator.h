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

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const objects::Model &model, int numCollectors);

class Simulator {
public:
  Simulator(RayTracer *tracer, objects::Model *model,
            generators::RayFactory *source,
            generators::RandomRayOffseter *offsetter)
      : tracer_(tracer), model_(model), source_(source),
        offsetter_(offsetter){};

  std::vector<float> run(float frequency, int numCollectors) {
    std::vector<std::unique_ptr<objects::EnergyCollector>> collectors =
        buildCollectors(*model_, numCollectors);
  }

private:
  RayTracer *tracer_;
  objects::Model *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;
};

#endif