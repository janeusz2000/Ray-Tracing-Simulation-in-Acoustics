#include "main/sceneManager.h"

SceneManager::SceneManager(Model *model,
                           const SimulationProperties &simulationProperties)
    : model_(model), simulationProperties_(simulationProperties),
      raytracer_(model), tracker_(simulationProperties.dataPath()),
      pointSpeaker_(simulationProperties.numOfRaySquared(),
                    simulationProperties.sourcePower(), model) {

  offseter_ = std::make_unique<generators::FakeOffseter>();
  referenceModel_ = Model::NewReferenceModel(model->sideSize());
}

void SceneManager::run() {
  Simulator simulator(&raytracer_, model_, &pointSpeaker_, offseter_.get(),
                      &tracker_, simulationProperties_.energyCollectionRules());

  // TODO: multicore energy calculation -> when core is available, use it
  // TODO: export data to Json
}
