#include "main/model.h"
#include "main/rayTracer.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "obj/generators.h"

#include <algorithm>
#include <string>

int main() {

  std::string path = "./models/simpleCube.obj";
  std::unique_ptr<Model> model = Model::NewLoadFromObjectFile(path.data());
  trackers::saveModelToJson("./data", model.get());
  trackers::JsonPositionTracker tracker("./data");
  collectionRules::LinearEnergyCollection energyCollectionRules;

  std::vector<float> frequencies = {100, 200, 300, 400, 500};
  SimulationProperties properties("./data", frequencies, &energyCollectionRules,
                                  500, 37, 1);

  SceneManager manager(model.get(), properties, &tracker);
  manager.run();
}
