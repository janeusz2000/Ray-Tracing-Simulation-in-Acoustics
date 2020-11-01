#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <vector>

// This class run simulation for specific frequency,
// Calculates pressure from energy in collector in collectors
// Prepares pressure values for export

class Simulator {
public:
  Simulator() = default; // TODO: resolve thsk folr later
  Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
            float simulationRadius, std::string_view objPath);

  // Im scared
  void run();
  void calculatePressure();

  Simulator &operator=(const Simulator &other);

private:
  void rayTrace(const core::Ray &ray, core::RayHitData *hitData, int depth);
  void printEnergy();

  generators::PointSource source_;
  generators::FakeRandomGen offsetGenerator_;
  SceneManager manager_;

  float frequency_;
  size_t numOfRaysPerRow_;
  std::vector<objects::TriangleObj *> model_;
  std::vector<objects::EnergyCollector *> collectors_;
};

#endif