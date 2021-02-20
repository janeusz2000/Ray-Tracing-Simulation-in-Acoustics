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

std::unordered_map<float, std::vector<float>> SceneManager::run() {

  std::vector<float> frequencies = simulationProperties_.frequencies();
  EnergiesPerFrequency outputEnergiesPerFrequency;

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

    Energies energies = simulator.run(freq, collectors);
    std::pair<float, Energies> energiesPerFrequency =
        std::make_pair<float, Energies>(std::move(freq), std::move(energies));
    outputEnergiesPerFrequency.insert(energiesPerFrequency);
    positionTracker_->save();
  }
  return outputEnergiesPerFrequency;
}
