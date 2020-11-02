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
  Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
            float simulationRadius, std::string_view objPath, int depth,
            float sourcePower);

  // Im scared
  void calculatePressure();
  void printEnergy();
  void run();
  void rayTrace(const core::Ray &ray, core::RayHitData *hitData, int depth);
  friend std::ostream &operator<<(std::ostream &os, const Simulator &sim);

  size_t numOfRaysPerRow() const { return numOfRaysPerRow_; }
  int depth() const { return depth_; }
  float frequency() const { return frequency_; }
  float sourcePower() const { return sourcePower_; }

private:
  // Disallow copy and assing
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  generators::PointSource source_;
  // TODO: I want to be able to change engine type when program is running
  generators::FakeRandomGen offsetGenerator_;
  SceneManager manager_;

  float frequency_, sourcePower_;
  size_t numOfRaysPerRow_;
  int depth_;
  std::vector<objects::TriangleObj *> model_;
  std::vector<objects::EnergyCollector *> collectors_;
};

#endif