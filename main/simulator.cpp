#include "main/simulator.h"

Simulator::Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
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

void Simulator::run() {
  // TODO: initialization of the simulation
}

void Simulator::calculatePressure() {
  // TODO: calculates all pressure from energy collectors
}

bool Simulator::rayTrace(const core::Ray &ray, core::RayHitData *hitData) {
  return false;
}
