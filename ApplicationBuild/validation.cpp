#include "main/model.h"
#include "main/rayTracer.h"
#include "main/resultsCalculation.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using EnergyPerTime = std::unordered_map<float, float>;
using Energy = std::vector<EnergyPerTime>;
using EnergyPerFrequency = std::unordered_map<float, Energy>;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

const int kSampleRate = 96e3;

const std::vector<float> frequencies = {100,  125,  160,  200,  250,
                                        315,  400,  500,  630,  800,
                                        1000, 1250, 1600, 2000, 2500};

float kDefaultModelSize = 1.0;

std::unique_ptr<CollectorBuilderInterface> summonBuilder(std::string_view key) {
  std::unordered_map<std::string_view, int> builderMap{
      {"xAxis", 0}, {"yAxis", 1}, {"xyAxis", 2}, {"geoDome", 3}};
  switch (builderMap.at(key)) {
  case 0:
    return std::make_unique<XAxisCollectorBuilder>();
  case 1:
    return std::make_unique<YAxisCollectorBuilder>();
  case 2:
    return std::make_unique<DoubleAxisCollectorBuilder>();
  case 3:
    return std::make_unique<GeometricDomeCollectorBuilder>();
  default:
    throw std::invalid_argument(std::string("Unrecognised collector shape: ") +
                                key.data());
  }
}
// ARGS MUST CONTAIN:
// #1 energy collector pattern type
// #2 raport path
// #3 model path
// #4 source Power
// #5 numOfCollectors
// #6 numOfRaysSquared
// #7 maxTracking

int main(int argc, char *argv[]) {
  std::vector<std::string> args(&argv[0], &argv[0 + argc]);
  std::string_view collectorShape = args[1];
  std::string_view raportPath = args[2];
  std::string_view modelPath = args[3];
  const float sourcePower = std::stof(args[4]);
  const int numOfCollectors = std::stoi(args[5]);
  const int numOfRaysSquared = std::stoi(args[6]);
  const int maxTracking = stoi(args[7]);

  std::cout << "starting validation for: " << modelPath << std::endl;
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(modelPath);

  trackers::FakePositionTracker positionTracker;
  trackers::FakeCollectorsTracker collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties(
      frequencies, sourcePower, numOfCollectors, numOfRaysSquared, maxTracking);
  SimulationProperties properties(&energyCollectionRules, basicProperties);

  std::unique_ptr<CollectorBuilderInterface> collectorBuilder =
      summonBuilder(collectorShape);
  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);
  std::unordered_map<float, Collectors> mapOfCollectors =
      manager.run(collectorBuilder.get());

  WaveObjectFactory waveFactory(kSampleRate);

  // ACOUSTIC PARAMETERS: declare and  append to |acousticParameters|
  // to involve it in simulation
  std::vector<ResultInterface *> acousticParameters;

  DiffusionCoefficient diffusion(&waveFactory);
  acousticParameters.push_back(&diffusion);

  trackers::ResultTracker resultTracker;

  for (ResultInterface *result : acousticParameters) {
    std::map<float, float> resultPerFrequency =
        result->getResults(mapOfCollectors);
    resultTracker.registerResult(result->getName(), resultPerFrequency);
  }

  trackers::Json raport = resultTracker.generateRaport();
  // TODO: This should not be member of restultsTracker()
  // make another interface for it;
  // resultTracker.compareDataToReference();
  resultTracker.saveRaport(raportPath.data());
}
