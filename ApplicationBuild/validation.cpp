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

const std::vector<float> frequencies = {500,  630,   800,   1000, 1250, 1600,
                                        2000, 2500,  3150,  4000, 5000, 6300,
                                        8000, 10000, 12500, 16000};

float kDefaultModelSize = 1.0;

// ARGS MUST CONTAIN:
// #1 raport path
// #2 model path
// #3 source Power
// #4 numOfCollectors
// #5 numOfRaysSquared
int main(int argc, char *argv[]) {
  std::vector<std::string> args(&argv[0], &argv[0 + argc]);
  std::string_view raportPath = args[1];
  std::string_view modelPath = args[2];
  const float sourcePower = std::stof(args[3]);
  const int numOfCollectors = std::stoi(args[4]);
  const int numOfRaysSquared = std::stoi(args[5]);

  std::cout << "starting validation for: " << modelPath << std::endl;
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(modelPath);

  trackers::FakePositionTracker positionTracker;
  trackers::FakeCollectorsTracker collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);
  SimulationProperties properties(&energyCollectionRules, basicProperties);
  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);

  std::unordered_map<float, Collectors> mapOfCollectors = manager.run();

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
