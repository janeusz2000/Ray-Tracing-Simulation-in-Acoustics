
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
const float sourcePower = 500; // [W]
const int numOfCollectors = 19;
const int numOfRaysSquared = 10;
const int numOfVisibleRaysSquared = 10;
const std::vector<float> frequencies = {500,  630,   800,   1000, 1250, 1600,
                                        2000, 2500,  3150,  4000, 5000, 6300,
                                        8000, 10000, 12500, 16000};

float kDefaultModelSize = 1.0;

std::string_view modelPath =
    "./validationDiffusors/2D_2m_6n_modulo7_200Hz_15stopni_5potega.obj";
std::string_view serverDataPath = "./server/data";

// ARGS MUST CONTAIN:
// frequency that will be used in simulation

int main(int argc, char *argv[]) {

  trackers::startSimulation();
  const std::vector<float> frequencies = {std::stof(argv[1])};
  trackers::DataExporter dataExporter;

  std::cout << "Starting tracking for the model at:\n\t" << modelPath << "\n"
            << "\nfor frequency: " << frequencies[0] << " Hz" << std::endl;

  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(modelPath);

  trackers::JsonSampledPositionTracker positionTracker(
      serverDataPath, numOfRaysSquared, numOfVisibleRaysSquared);
  trackers::CollectorsTrackerToJson collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);

  SimulationProperties properties(&energyCollectionRules, basicProperties);
  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);

  GeometricDomeCollectorBuilder collectorBuilder;
  std::unordered_map<float, Collectors> mapOfCollectors =
      manager.run(&collectorBuilder);

  std::cout << "Ray tracing of the object at: \n\t" << modelPath
            << "\nhas finishied!" << std::endl;

  dataExporter.saveModelToJson(serverDataPath, model.get(),
                               /*referenceModel=*/false);

  positionTracker.switchToReferenceModel();
  std::unique_ptr<Model> referenceModel =
      Model::NewReferenceModel(model->sideSize());

  SceneManager referenceManager(referenceModel.get(), properties,
                                &positionTracker, &collectorsTracker);

  std::unordered_map<float, Collectors> mapOfReferenceCollectors =
      referenceManager.run(&collectorBuilder);
  dataExporter.saveModelToJson(serverDataPath, referenceModel.get(),
                               /*referenceModel=*/true);

  trackers::endSimulation();
}
