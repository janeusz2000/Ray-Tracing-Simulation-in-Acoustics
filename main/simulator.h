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
// Writes results to txt file

class Simulator {
public:
  Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
            float simulationRadius, std::string_view objPath);

  // Im scared
  void run();
  void calculatePressure();

private:
  Simulator() = delete;
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

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