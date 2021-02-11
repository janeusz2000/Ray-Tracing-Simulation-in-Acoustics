#include "main/simulator.h"

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const ModelInterface &model, int numCollectors) {

  if (model.empty()) {
    throw std::invalid_argument("Empty model");
  }
  if (numCollectors < 4) {
    std::stringstream ss;
    ss << "numCollectors: " << numCollectors << " is less then 4";
    throw std::invalid_argument(ss.str());
  }

  // We distribute collectors along 4 quarter circles above the model, so
  // |numCollectors| has to be divisible by 4. Or |numCollectors|-1 has to be
  // divisible by 4 (if it is odd), in which case there is one more collector
  // placed right above the model.
  if (numCollectors % 4 != 0 && (numCollectors - 1) % 4 != 0) {
    std::stringstream ss;
    ss << "numCollectors or numCollectors-1 has to be divisible by 4, got "
       << "numCollectors = " << numCollectors;
    throw std::invalid_argument(ss.str());
  }

  // We are placing collectors on two half-circles, such that distance between
  // each collector on a given circle is equal to their radius.

  // For even number of collectors, on each half-circle we place N/2 collectors
  // in total, with 2 collectors at the ground level. This divides the PI into
  // N/2-1 segments.
  // For odd number of collectors, we do the same and place 1 more collector at
  // the top, so we divide PI into (N - 1) / 2 segments.
  // Those two cases can be shorten into:
  const int numCollectorReminder = numCollectors % 2;
  const float angleBetweenCollectors =
      2 * constants::kPi / (numCollectors + numCollectorReminder - 2);

  // radius of the two half-circles  is equal to the 4 times the biggest
  // dimension of the model and must be at least equal to 4, which is equal to
  // SphereWall radius.
  const float collectorSphereRadius = getSphereWallRadius(model);

  // The radius of one collector (distance between collectors) can be then
  // calculated with Law of cosines, as R *sqrt(2 - 2 *
  // cos(angleBetweenCollectors)).
  float energyCollectorRadius =
      collectorSphereRadius *
      std::sqrt(2.0f - 2 * std::cos(angleBetweenCollectors));

  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors;
  // When Num Collectors is not even, we need to put one collector at (0, 0,
  // collectorSphereRadius)
  if (numCollectorReminder == 1) {
    energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
        core::Vec3(0, 0, collectorSphereRadius), energyCollectorRadius));
  }
  // and decrease number remaining collectors to create remaining ones
  const int numToGo = numCollectors - (numCollectorReminder);

  // Remaining energy collectors:
  std::vector<core::Vec3> origins;
  for (int times = 0; times * 4 < numToGo; ++times) {

    float groundCoordinate =
        collectorSphereRadius * std::cos(times * angleBetweenCollectors);
    float zCoordinate =
        collectorSphereRadius * std::sin(times * angleBetweenCollectors);

    origins = {core::Vec3(groundCoordinate, 0, zCoordinate),
               core::Vec3(0, -groundCoordinate, zCoordinate),
               core::Vec3(-groundCoordinate, 0, zCoordinate),
               core::Vec3(0, groundCoordinate, zCoordinate)};

    for (const core::Vec3 &origin : origins) {
      energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
          origin, energyCollectorRadius));
    }
  }
  return energyCollectors;
}

const float getSphereWallRadius(const ModelInterface &model) {
  return std::max(constants::kSimulationHeight / 2.0f,
                  4 * std::max(model.height(), model.sideSize()));
}

// TODO: create class hierarchy for this function, to try different algorithms
// of collection.
void collectEnergy(
    std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors,
    core::RayHitData *hitData) {

  core::Vec3 reachedPosition = hitData->collisionPoint();
  for (auto &energyCollector : collectors) {
    if (energyCollector->isVecInside(reachedPosition)) {
      float distanceToOrigin =
          (energyCollector->getOrigin() - reachedPosition).magnitude();

      // The closer ray hits origin of the energy Collector, the more energy
      // energyCollector collects.
      float energyRatio = 1 - distanceToOrigin / energyCollector->getRadius();
      energyCollector->addEnergy(energyRatio * hitData->energy());

      // TODO: Phase impact on the collect energy. (new class)
    }
  }
}

std::vector<float> Simulator::run(float frequency, int numCollectors) {
  std::vector<std::unique_ptr<objects::EnergyCollector>> collectors =
      buildCollectors(*model_, numCollectors);

  objects::SphereWall sphereWall(getSphereWallRadius(*model_));

  core::Ray currentRay;
  core::RayHitData hitData;

  while (source_->genRay(&currentRay)) {

    // Ray-Trace until Rays excape the model.
    positionTracker_->initializeNewTracking();
    RayTracer::TraceResult hitResult =
        RayTracer::TraceResult::WENT_OUTSIDE_OF_SIMULATION_SPACE;
    do {
      hitResult = tracer_->rayTrace(currentRay, frequency, &hitData);
      currentRay = tracer_->getReflected(&hitData);

      positionTracker_->addNewPositionToCurrentTracking(hitData);

    } while (hitResult == RayTracer::TraceResult::HIT_TRIANGLE);

    if (!sphereWall.hitObject(currentRay, frequency, &hitData)) {
      std::stringstream ss;
      ss << "Ray went outside of the simulation! \n"
         << "Ray Origin: " << hitData.origin() << "\n"
         << "Ray Direction: " << hitData.direction() << "\n"
         << "Collision Point: " << hitData.collisionPoint() << "\n"
         << "Sphere Wall Radius: " << sphereWall.getRadius();
      throw std::logic_error(ss.str());
    };

    positionTracker_->addNewPositionToCurrentTracking(hitData);
    positionTracker_->endCurrentTracking();

    collectEnergy(collectors, &hitData);
  }

  return getEnergyFromGivenCollectors(collectors);
}

std::vector<float> Simulator::getEnergyFromGivenCollectors(
    const std::vector<std::unique_ptr<objects::EnergyCollector>> &collectors) {
  std::vector<float> output;
  output.reserve(collectors.size());
  for (auto &collector : collectors) {
    output.push_back(collector->getEnergy());
  }
  return output;
}
