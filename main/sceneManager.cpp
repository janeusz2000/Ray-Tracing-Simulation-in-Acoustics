#include "main/sceneManager.h"

SceneManager::SceneManager(Model *model,
                           const SimulationProperties &simulationProperties,
                           trackers::PositionTrackerInterface *tracker)
    : model_(model), simulationProperties_(simulationProperties),
      raytracer_(model), tracker_(tracker) {

  offseter_ = std::make_unique<generators::FakeOffseter>();
  referenceModel_ = Model::NewReferenceModel(model->sideSize());
}

std::vector<std::vector<float>> SceneManager::run() {

  std::vector<float> frequencies = simulationProperties_.frequencies();
  energiesPerFrequency outputEnergiesPerFrequency;

  for (float freq : frequencies) {
    generators::PointSpeakerRayFactory pointSpeaker(
        simulationProperties_.numOfRaySquared(),
        simulationProperties_.sourcePower(), model_);
    Simulator simulator(&raytracer_, model_, &pointSpeaker, offseter_.get(),
                        tracker_,
                        simulationProperties_.energyCollectionRules());
    Collectors collectors =
        buildCollectors(model_, simulationProperties_.numOfCollectors());

    outputEnergiesPerFrequency.push_back(simulator.run(freq, collectors));
    tracker_->save();
  }
  return outputEnergiesPerFrequency;
}
