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

void Simulator::rayTrace(const core::Ray &ray, core::RayHitData *hitData,
                         int depth) {
  core::RayHitData closestHitdata;

  if (depth > 0) {
    bool continueRecursion = false;

    for (const auto &obj : model_) {

      if (obj->hitObject(ray, frequency_, hitData)) {
        if ((ray.getOrigin() - hitData->origin()).magnitude() <
            (ray.getOrigin() - closestHitdata.origin()).magnitude()) {

          closestHitdata = *hitData;
          continueRecursion = true;
        }
      }
    }
    if (continueRecursion) {

      // Reflected direction
      core::Vec3 newDir =
          ray.getDirection() -
          2 * (closestHitdata.normal().scalarProduct(ray.getDirection()) *
               closestHitData.normal());

      core::Ray reflectedRay(closestHitData.collisionPoint(), newDir);
      rayTrace(reflectedRay, hitData, depth - 1);
    }
  }
}
