#include "main/sceneManager.h"

SceneManager::SceneManager(Model *model,
                           const SimulationProperties &simulationProperties,
                           trackers::PositionTrackerInterface *tracker)
    : model_(model), simulationProperties_(simulationProperties),
      raytracer_(model), tracker_(tracker) {

  offseter_ = std::make_unique<generators::FakeOffseter>();
  referenceModel_ = Model::NewReferenceModel(model->sideSize());
}

void SceneManager::run() {
  using energies = std::vector<float>;

  std::vector<float> frequencies = simulationProperties_.frequencies();
  std::vector<energies> futureVec;

  for (float freq : frequencies) {
    generators::PointSpeakerRayFactory pointSpeaker(
        simulationProperties_.numOfRaySquared(),
        simulationProperties_.sourcePower(), model_);
    Simulator simulator(&raytracer_, model_, &pointSpeaker, offseter_.get(),
                        tracker_,
                        simulationProperties_.energyCollectionRules());
    Collectors collectors =
        buildCollectors(model_, simulationProperties_.numOfCollectors());

    exportCollectorsToJson(collectors, "./data/energyCollectors.json");

    futureVec.push_back(simulator.run(freq, collectors));
    tracker_->save();
  }

  std::cout << "===== RAPORT =====" << std::endl;
  for (size_t index = 0; index < frequencies.size(); ++index) {
    std::cout << "Frequency: " << frequencies[index] << ", data: [";
    for (const auto &energy : futureVec[index]) {
      std::cout << energy << ", ";
    }
    std::cout << "]" << std::endl;
  }
}
