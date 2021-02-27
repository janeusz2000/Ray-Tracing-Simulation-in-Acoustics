#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

int main() {

  std::string dataPath = "./data";
  std::string path = "./models/normalDiffusor.obj";
  std::vector<float> frequencies = {50,   100,  200,  400,   800,
                                    1600, 3200, 6400, 12800, 25600};
  float sourcePower = 500; // [W]
  int numOfCollectors = 37;
  int numOfRaysSquared = 20;
  int numOfVisibleRays = 10;

  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());

  trackers::saveModelToJson(dataPath, model.get());
  trackers::JsonSampledPositionTracker positionTracker(
      dataPath, numOfRaysSquared, numOfVisibleRays);
  trackers::CollectorsTrackerToJson collectorsTracker;
  collectionRules::NonLinearEnergyCollection energyCollectionRules;

  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);
  SimulationProperties properties(&energyCollectionRules, basicProperties);

  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);
  std::unordered_map<float, std::vector<float>> results = manager.run();
  trackers::saveResultsAsJson(dataPath, results);
}
