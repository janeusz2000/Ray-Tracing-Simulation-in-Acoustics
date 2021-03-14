#include "main/model.h"
#include "main/rayTracer.h"
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

int main() {

  trackers::startSimulation();
  std::string dataPath = "./data";
  std::string path = "./models/normalDiffusor.obj";
  std::vector<float> frequencies = {500,  630,   800,   1000, 1250, 1600,
                                    2000, 2500,  3150,  4000, 5000, 6300,
                                    8000, 10000, 12500, 16000};
  float sourcePower = 1000; // [W]
  int numOfCollectors = 37;
  int numOfRaysSquared = 100;
  int numOfVisibleRays = 10;

  trackers::DataExporter dataExporter;
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());
  dataExporter.saveModelToJson(dataPath, model.get());

  trackers::JsonSampledPositionTracker positionTracker(
      dataPath, numOfRaysSquared, numOfVisibleRays);

  trackers::CollectorsTrackerToJson collectorsTracker;
  collectionRules::NonLinearEnergyCollection energyCollectionRules;

  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);
  SimulationProperties properties(&energyCollectionRules, basicProperties);
  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);
  EnergyPerFrequency results = manager.run();
  dataExporter.saveResultsAsJson(dataPath, results);

  positionTracker.switchToReferenceModel();
  std::unique_ptr<Model> referenceModel =
      Model::NewReferenceModel(model->sideSize());

  SceneManager referenceManager(referenceModel.get(), properties,
                                &positionTracker, &collectorsTracker);
  EnergyPerFrequency referenceResults = referenceManager.run();
  dataExporter.saveResultsAsJson(dataPath, referenceResults,
                                 /*referenceModel=*/true);
  dataExporter.saveModelToJson(dataPath, referenceModel.get(),
                               /*referenceModel=*/true);
  trackers::endSimulation();
}
