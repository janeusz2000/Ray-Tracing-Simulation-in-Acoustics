#include "main/simulator.h"

std::vector<objects::EnergyCollector>
buildCollectors(const ModelInterface *model, int numCollectors) {

  if (model->empty()) {
    throw std::invalid_argument("Model cannot be Empty!");
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
  const float collectorSphereRadius = getSphereWallRadius(*model);

  // The radius of one collector (distance between collectors) can be then
  // calculated with Law of cosines, as R *sqrt(2 - 2 *
  // cos(angleBetweenCollectors)).
  float energyCollectorRadius =
      collectorSphereRadius *
      std::sqrt(2.0f - 2 * std::cos(angleBetweenCollectors));

  std::vector<objects::EnergyCollector> energyCollectors;
  energyCollectors.reserve(numCollectors);
  // When Num Collectors is not even, we need to put one collector at (0, 0,
  // collectorSphereRadius)
  if (numCollectorReminder == 1) {
    energyCollectors.push_back(objects::EnergyCollector(
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
      energyCollectors.push_back(
          objects::EnergyCollector(origin, energyCollectorRadius));
    }
  }
  return energyCollectors;
}

// exports |energyCollectors| as string representation to |path|
void exportCollectorsToJson(
    const std::vector<std::unique_ptr<objects::EnergyCollector>>
        &energyCollectors,
    std::string_view path) {

  std::ofstream outFile(path.data());
  if (!outFile.good()) {
    std::stringstream errorStream;
    errorStream << "File at path: " << path.data() << ", doesn't exist!";
    throw std::invalid_argument(errorStream.str());
  }

  using Json = nlohmann::json;
  Json outArray = Json::array();
  int currentCollectorNumber = 0;
  for (const auto &collector : energyCollectors) {
    core::Vec3 collectorOrigin = collector->getOrigin();
    float radius = collector->getRadius();
    Json energyCollector = {{"number", currentCollectorNumber},
                            {"x", collectorOrigin.x()},
                            {"y", collectorOrigin.y()},
                            {"z", collectorOrigin.z()},
                            {"radius", radius}};
    outArray.push_back(energyCollector);
    ++currentCollectorNumber;
  }

  outFile << outArray.dump(1);
  outFile.close();
}

const float getSphereWallRadius(const ModelInterface &model) {
  return std::max(constants::kSimulationHeight / 2.0f,
                  4 * std::max(model.height(), model.sideSize()));
}

void Simulator::run(float frequency,
                    std::vector<objects::EnergyCollector> &collectors,
                    std::promise<energiesPerFrequency> &promise) {

  std::cout << "THREAD: frequency: " << frequency << "started!" << std::endl;
  objects::SphereWall sphereWall(getSphereWallRadius(*model_));

  core::Ray currentRay;
  core::RayHitData hitData;

  while (source_->genRay(&currentRay)) {

    // TODO: dont track all rays -> sample them, to increase performance and
    // readability
    // Ray-Trace until Rays excape the model.
    // positionTracker_->initializeNewTracking();
    RayTracer::TraceResult hitResult =
        RayTracer::TraceResult::WENT_OUTSIDE_OF_SIMULATION_SPACE;
    do {
      hitResult = tracer_->rayTrace(currentRay, frequency, &hitData);
      currentRay = tracer_->getReflected(&hitData);

      // positionTracker_->addNewPositionToCurrentTracking(hitData);

    } while (hitResult == RayTracer::TraceResult::HIT_TRIANGLE);

    if (sphereWall.hitObject(currentRay, frequency, &hitData)) {
      // positionTracker_->addNewPositionToCurrentTracking(hitData);
      // positionTracker_->endCurrentTracking();
    }

    energyCollectionRules_->collectEnergy(collectors, &hitData);
  }

  promise.set_value(std::make_pair<std::vector<float>, float>(
      getEnergyFromGivenCollectors(collectors), std::move(frequency)));
  std::cout << "THREAD: frequency: " << frequency << "ended!" << std::endl;
}

std::vector<float> Simulator::getEnergyFromGivenCollectors(
    const std::vector<objects::EnergyCollector> &collectors) {
  std::vector<float> output;
  output.reserve(collectors.size());
  for (auto &collector : collectors) {
    output.push_back(collector.getEnergy());
  }
  return output;
}
