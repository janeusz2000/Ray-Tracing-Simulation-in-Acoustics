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
  Simulator()
      : Simulator(constants::kSkipFreq, constants::kDefaultNumRaysPerRow,
                  constants::kDefaultCollectorPopulation,
                  constants::kDefaultSimulationRadius, constants::kSkipPath,
                  constants::kDefaultDepth, constants::kDefaultSourcePower){};
  Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
            float simulationRadius, std::string_view objPath, int depth,
            float sourcePower);

  // Im scared
  void run();
  void calculatePressure();
  void printEnergy();

private:
  // Disallow copy and assing
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  void rayTrace(const core::Ray &ray, core::RayHitData *hitData, int depth);

  generators::PointSource source_;
  // TODO: I want to be able to change engine during the program
  generators::FakeRandomGen offsetGenerator_;
  SceneManager manager_;

  float frequency_, sourcePower_;
  size_t numOfRaysPerRow_;
  int depth_;
  std::vector<objects::TriangleObj *> model_;
  std::vector<objects::EnergyCollector *> collectors_;
};

#endif