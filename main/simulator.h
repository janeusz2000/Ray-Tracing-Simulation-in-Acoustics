#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/sceneManager.h"
#include "obj/objects.h"

#include <vector>
// This class run simulations for specific frequency,
// Calculates pressure from energy in collector in collectors
// Writes results to txt file

// TODO: how to pass path to sceneManager?

class Simulator {
public:
  Simulator(float frequency, size_t rayNumPerRow, int collectors,
            float simulationRadius)
      : frequency_(frequency), rayNumPerRow_(rayNumPerRow),
        manager_(SceneManager(collectors, simulationRadius)) {
    // model_ = manager_.getModelTriangles();  // TODO: no implementation yet
    model_ = manager_.getReferencePlate(); // TODO: remove
    collectors_ = manager_.getEnergyCollectors();
  };

  // Im scared
  void run();
  void calculatePressure();

private:
  Simulator() = delete;
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  float frequency_;
  size_t rayNumPerRow_;
  SceneManager manager_;
  std::vector<objects::TriangleObj *> model_;
  std::vector<objects::EnergyCollector *> collectors_;
};

#endif