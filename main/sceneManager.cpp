#include "main/sceneManager.h"

SceneManager::SceneManager(
    Model *model, const std::vector<float> &frequencies,
    std::string_view dataPath,
    collectionRules::CollectEnergyInterface *energyCollectionRules,
    float sourcePower, int numOfCollectors, int numOfRaysSquared)
    : model_(model), frequencies_(frequencies),
      energyCollectionRules_(energyCollectionRules),
      numOfCollectors_(numOfCollectors), raytracer_(model),
      tracker_(dataPath.data()),
      pointSpeaker_(numOfRaysSquared, sourcePower, model) {

  offseter_ = std::make_unique<generators::FakeOffseter>();
  referenceModel_ = Model::NewReferenceModel(model->sideSize());
}

void SceneManager::run() {
  Simulator simulator(&raytracer_, model_, &pointSpeaker_, offseter_.get(),
                      &tracker_, energyCollectionRules_);

  // TODO: multicore energy calculation -> when core is available, use it
  // TODO: export data to Json
}
