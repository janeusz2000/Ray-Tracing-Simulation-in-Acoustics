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
const float sourcePower = 1000;
const int numOfCollectors = 37;
const int numOfRaysSquared = 200;
const std::vector<float> frequencies = {500,  630,   800,   1000, 1250, 1600,
                                        2000, 2500,  3150,  4000, 5000, 6300,
                                        8000, 10000, 12500, 16000};

std::string_view raportPath = "./data";
std::string_view referenceDataPath;

float kDefaultModelSize = 1.0;

int main() {

  std::unique_ptr<Model> model = Model::NewReferenceModel(kDefaultModelSize);

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
