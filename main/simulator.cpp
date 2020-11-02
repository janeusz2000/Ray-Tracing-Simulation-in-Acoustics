#include "main/simulator.h"

Simulator::Simulator(float frequency, size_t numOfRaysPerRow, int collectors,
                     float simulationRadius, std::string_view objPath,
                     int depth, float sourcePower)
    : frequency_(frequency), numOfRaysPerRow_(numOfRaysPerRow_),
      manager_(SceneManager(collectors, simulationRadius)), depth_(depth),
      sourcePower_(sourcePower) {
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

  source_ = generators::PointSource(frequency_, numOfRaysPerRow_,
                                    manager_.getSmapleSize(), &offsetGenerator_,
                                    simulationRadius, sourcePower);
}

void Simulator::run() {
  for (size_t xIter = 0; xIter < numOfRaysPerRow_; ++xIter) {
    for (size_t yIter = 0; yIter < numOfRaysPerRow_; ++yIter) {
      core::Ray currentRay = source_.generateRay(xIter, yIter);
      core::RayHitData hitData;
      rayTrace(currentRay, &hitData, depth_);
    }
  }
}

void Simulator::calculatePressure() {
  // TODO: calculates all pressure from energy collectors
}

void Simulator::rayTrace(const core::Ray &ray, core::RayHitData *hitData,
                         int depth) {
  core::RayHitData closestHitData;

  if (depth > 0) {
    bool continueRecursion = false;

    // Finding the closest object that ray hits
    for (const auto &obj : model_) {
      if (obj->hitObject(ray, frequency_, hitData)) {
        if ((ray.getOrigin() - hitData->origin()).magnitude() <
            (ray.getOrigin() - closestHitData.origin()).magnitude()) {
          closestHitData = *hitData;
          continueRecursion = true;
        }
      }
    }

    // if sphereWall hit point is closer to ray then
    // Triangle hits or does ray hit Sphere wall only.
    if (manager_.getSphereWall().hitObject(ray, frequency_, hitData)) {
      if ((ray.getOrigin() - hitData->origin()).magnitude() <
          (ray.getOrigin() - closestHitData.origin()).magnitude()) {

        // Find the closest energy collector to
        // the point where sphereWall is hit
        float distance = std::numeric_limits<float>::max();
        int closestCollectorIndex;
        for (size_t collectorIndex = 0; collectorIndex < collectors_.size();
             ++collectorIndex) {
          float tempDistance = (collectors_[collectorIndex]->getOrigin() -
                                hitData->collisionPoint())
                                   .magnitude();
          if (tempDistance < distance) {
            distance = tempDistance;
            closestCollectorIndex = collectorIndex;
          }
        }

        // TODO: energy distribution between collectors
        // add energy to the closest collector
        collectors_[closestCollectorIndex]->addEnergy(ray.getEnergy());
      }
    }

    // if triangle is hit, then we continue ray tracing
    if (continueRecursion) {

      // Snell's law applied for reflected ray
      core::Vec3 newDir =
          ray.getDirection() -
          2 * (closestHitData.normal().scalarProduct(ray.getDirection()) *
               closestHitData.normal());

      core::Ray reflectedRay(closestHitData.collisionPoint(), newDir);
      rayTrace(reflectedRay, hitData, depth - 1);
    }
  }
}

void Simulator::printEnergy() {
  for (const auto &col : collectors_) {
    std::cout << col->getEnergy() << "\n";
  }
  std::cout << std::flush;
}
