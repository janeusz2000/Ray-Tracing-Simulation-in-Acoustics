#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "main/rayTracer.h"

#include <cmath>
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
// model. Radius of an energy collector is equal to distance between two
// collectors.

// Throws std::invalid_argument when |numCollectors| < 4 or when |numCollectors|
// or |numCollectors|-1 is not divisible by 4.
std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const ModelInterface &model, int numCollectors);

class Simulator {
public:
  Simulator(RayTracer *tracer, ModelInterface *model,
            generators::RayFactory *source,
            generators::RandomRayOffseter *offsetter)
      : tracer_(tracer), model_(model), source_(source),
        offsetter_(offsetter){};

  std::vector<float> run(float frequency, int numCollectors);

private:
  RayTracer *tracer_;
  ModelInterface *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;
};

#endif