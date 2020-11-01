#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <sstream>
#include <string_view>
#include <vector>

// This class run simulation for specific frequency,
// Calculates pressure from energy in collector in collectors
// Writes results to txt file

class Simulator {
public:
  Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
            float simulationRadius, std::string_view objPath)
      : frequency_(frequency), numOfRaysPerRow_(numOfRaysPerRow_),
        manager_(SceneManager(collectors, simulationRadius)) {
    // TODO: no implementation yet
    // model_ = manager_.getModelTriangles();

    if (!manager_.loadModel(objPath)) {
      std::stringstream ss;
      ss << "Invalid model path";
      throw std::invalid_argument(ss.str());
    }

    // TODO: remove
    model_ = manager_.getReferencePlate();
    collectors_ = manager_.getEnergyCollectors();

    source_ =
        generators::PointSource(frequency_, numOfRaysPerRow_,
                                manager_.getSmapleSize(), &offsetGenerator_);
  }

  // Im scared
  void run();
  void calculatePressure();

private:
  Simulator() = delete;
  Simulator(const Simulator &) = delete;
  Simulator &operator=(const Simulator &) = delete;

  generators::PointSource source_;
  generators::FakeRandomGen offsetGenerator_;
  float frequency_;
  size_t numOfRaysPerRow_;
  SceneManager manager_;
  std::vector<objects::TriangleObj *> model_;
  std::vector<objects::EnergyCollector *> collectors_;
};

#endif