#include "main/sceneManager.h"

SceneManager::SceneManager(
    Model *model, const SimulationProperties &simulationProperties,
    trackers::PositionTrackerInterface *positionTracker,
    trackers::CollectorsTrackerInterface *collectorTracker)
    : model_(model), simulationProperties_(simulationProperties),
      raytracer_(model), positionTracker_(positionTracker),
      collectorsTracker_(collectorTracker) {

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
                        positionTracker_,
                        simulationProperties_.energyCollectionRules());
    Collectors collectors =
        buildCollectors(model_, simulationProperties_.numOfCollectors());

    collectorsTracker_->save(collectors, "./data");

    outputEnergiesPerFrequency.push_back(simulator.run(freq, collectors));
    positionTracker_->save();
  }
  return outputEnergiesPerFrequency;
}
