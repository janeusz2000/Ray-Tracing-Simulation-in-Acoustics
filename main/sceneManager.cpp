#include "main/sceneManager.h"

BasicSimulationProperties::BasicSimulationProperties(
    const std::vector<float> &frequencies, float sourcePower,
    int numOfCollectors, int numOfRaysSquared)
    : frequencies(frequencies), sourcePower(sourcePower),
      numOfCollectors(numOfCollectors), numOfRaysSquared(numOfRaysSquared) {

  if (frequencies.empty()) {
    throw std::invalid_argument(
        "Frequencies in BasicSimulationProperties data class cannot be empty!");
  }
  if (sourcePower < 0) {
    std::stringstream errorStream;
    errorStream << "Source Power given to: \n" << *this << "cannot be < 0! \n";
    throw std::invalid_argument(errorStream.str());
  }
  if (numOfCollectors < 4) {
    std::stringstream errorStream;
    errorStream << "Number of collectors in: \n"
                << *this << "cannot be less then 4!";
    throw std::invalid_argument(errorStream.str());
  }
  if (numOfCollectors % 4 != 0 && (numOfCollectors - 1) % 4 != 0) {
    std::stringstream errorStream;
    errorStream << "number of collectors or number of collectors -1 in: \n"
                << *this << "must be divisable by 4!";
    throw std::invalid_argument(errorStream.str());
  }
  if (numOfRaysSquared < 1) {
    std::stringstream errorStream;
    errorStream << "numOfRaysSquared in: \n"
                << *this << "must be greater then 0!";
  }
}

void BasicSimulationProperties::printItself(std::ostream &os) const noexcept {
  os << "BasicSimulationProperties data class"
     << "\n"
     << "frequencies: ";
  for (const float &freq : frequencies) {
    os << freq << ", ";
  }
  os << "\n"
     << "Source Power: " << sourcePower << "\n"
     << "Number Of Collectors: " << numOfCollectors << "\n"
     << "Number of Rays Squared: " << numOfRaysSquared << "\n";
}

SimulationProperties::SimulationProperties(
    collectionRules::CollectEnergyInterface *energyCollectionRules,
    const BasicSimulationProperties &basicSimulationProperties)
    : energyCollectionRules_(energyCollectionRules),
      basicSimulationProperties_(basicSimulationProperties){};

void SimulationProperties::printItself(std::ostream &os) const noexcept {
  os << "SimulationProperties\n"
     << "- Energy collection rules: \n"
     << *(energyCollectionRules_) << "- Basic Simulation Properties: \n"
     << basicSimulationProperties_;
}

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

void SceneManager::printItself(std::ostream &os) const noexcept {
  os << "SCENE MANAGER\n"
     << "Model: " << *(model_) << "\n"
     << "Simulation properties: " << simulationProperties_ << "\n"
     << "Ray Tracer: " << raytracer_ << "\n"
     << "Position Tracker: " << *(positionTracker_) << "\n"
     << "Collectors Tracker: " << *(collectorsTracker_) << "\n"
     << "Offseter: " << *(offseter_) << "\n"
     << "Reference Model: " << *(referenceModel_);
}

std::unordered_map<float, std::vector<float>> SceneManager::run() {
  std::vector<float> frequencies =
      simulationProperties_.basicSimulationProperties().frequencies;
  EnergiesPerFrequency outputEnergiesPerFrequency;

  for (float freq : frequencies) {
    generators::PointSpeakerRayFactory pointSpeaker(
        simulationProperties_.basicSimulationProperties().numOfRaysSquared,
        simulationProperties_.basicSimulationProperties().sourcePower, model_);
    Simulator simulator(&raytracer_, model_, &pointSpeaker, offseter_.get(),
                        positionTracker_,
                        simulationProperties_.energyCollectionRules());
    Collectors collectors = buildCollectors(
        model_,
        simulationProperties_.basicSimulationProperties().numOfCollectors);

    collectorsTracker_->save(collectors, "./data");

    Energies energies = simulator.run(freq, collectors);
    std::pair<float, Energies> energiesPerFrequency =
        std::make_pair<float, Energies>(std::move(freq), std::move(energies));
    outputEnergiesPerFrequency.insert(energiesPerFrequency);
    positionTracker_->save();
  }
  return outputEnergiesPerFrequency;
}
