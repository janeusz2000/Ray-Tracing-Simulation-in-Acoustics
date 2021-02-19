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
  using energies = std::vector<float>;
  using energiesPerFrequency = std::pair<energies, float>;
  using Collectors = std::vector<objects::EnergyCollector>;

  Simulator simulator(&raytracer_, model_, &pointSpeaker_, offseter_.get(),
                      &tracker_, simulationProperties_.energyCollectionRules());
  std::vector<float> frequencies = simulationProperties_.frequencies();

  const size_t processorsNumber = std::thread::hardware_concurrency();
  if (processorsNumber == 0) {
    throw std::invalid_argument("hardware concurrency is not supported");
  }
  size_t numberOfUsedProcessors = 0;

  std::vector<std::future<energiesPerFrequency>> futureVec;
  std::vector<std::thread> threads;
  auto currentFrequencyIterator = frequencies.cbegin();
  while (true) {
    while (numberOfUsedProcessors < processorsNumber) {
      float frequency = *currentFrequencyIterator;
      int numOfCollectors = simulationProperties_.numOfCollectors();
      Collectors collectors = buildCollectors(model_, numOfCollectors);

      std::promise<energiesPerFrequency> promise;
      futureVec.push_back(promise.get_future());
      threads.push_back(std::thread(&Simulator::run, &simulator, frequency,
                                    collectors, promise));

      ++currentFrequencyIterator;
      ++numberOfUsedProcessors;
    }

    for (std::thread &thread : threads) {
      if (thread.joinable()) {
        thread.join();
        --numberOfUsedProcessors;
      }
    }

    if (numberOfUsedProcessors == 0 &&
        currentFrequencyIterator == frequencies.cend()) {
      break;
    }
  }

  for (auto &future : futureVec) {
    energiesPerFrequency data = future.get();
    std::cout << "Frequency: " << data.second << ", data: [ ";
    for (const auto &energy : data.first) {
      std::cout << energy << ", ";
    }
    std::cout << "]" << std::endl;
  }
}
