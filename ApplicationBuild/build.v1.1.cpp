#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>
#include <vector>

int main() {

  std::string path = "./models/monkeyfull.obj";
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());

  trackers::saveModelToJson("./data", model.get());
  trackers::JsonPositionTracker positionTracker("./data");
  trackers::CollectorsTrackerToJson collectorsTracker;

  collectionRules::LinearEnergyCollection energyCollectionRules;

  std::vector<float> frequencies = {100, 200, 300, 400, 500};
  float sourcePower = 500; // [W]
  int numOfCollectors = 37;
  int numOfRaysSquared = 20;

  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);
  SimulationProperties properties(&energyCollectionRules, basicProperties);

  SceneManager manager(model.get(), properties, &positionTracker,
                       &collectorsTracker);
  manager.run();
}
