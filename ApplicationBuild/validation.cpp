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

const int kSampleRate = 96e3;
const float sourcePower = 1000;
const int numOfCollectors = 37;
const int numOfRaysSquared = 100;
const int numOfVisibleRays = 10;
const std::vector<float> frequencies = {500,  630,   800,   1000, 1250, 1600,
                                        2000, 2500,  3150,  4000, 5000, 6300,
                                        8000, 10000, 12500, 16000};
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

  // ACOUSTIC PARAMETERS
  DiffusionCoefficient diffusion(&waveFactory);
  std::vector<ResultInterface *> acousticParameters;
  acousticParameters.push_back(&diffusion);
  // TODO: calculate diffusion coefficient from given std::unordered_map<float,
  // collectors>
}

// int main() {

//   trackers::startSimulation();
//   std::string dataPath = "./data";
//   std::string path = "./models/monkeyfull.obj";

//   trackers::DataExporter dataExporter;
//   std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());
//   dataExporter.saveModelToJson(dataPath, model.get());

//   trackers::JsonSampledPositionTracker positionTracker(
//       dataPath, numOfRaysSquared, numOfVisibleRays);

//   trackers::CollectorsTrackerToJson collectorsTracker;
//   collectionRules::NonLinearEnergyCollection energyCollectionRules;

//   EnergyPerFrequency results = manager.run();
//   dataExporter.saveResultsAsJson(dataPath, results);

//   positionTracker.switchToReferenceModel();
//   std::unique_ptr<Model> referenceModel =
//       Model::NewReferenceModel(model->sideSize());

//   SceneManager referenceManager(referenceModel.get(), properties,
//                                 &positionTracker, &collectorsTracker);
//   EnergyPerFrequency referenceResults = referenceManager.run();
//   dataExporter.saveResultsAsJson(dataPath, referenceResults,
//                                  /*referenceModel=*/true);
//   dataExporter.saveModelToJson(dataPath, referenceModel.get(),
//                                /*referenceModel=*/true);
//   trackers::endSimulation();
// }
