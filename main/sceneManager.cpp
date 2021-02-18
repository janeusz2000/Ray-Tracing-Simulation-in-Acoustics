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

// TODO: fix nullptr promise
// TODO: fix iterations
// TODO: code is dirty -> clean it
void SceneManager::run() {
  Simulator simulator(&raytracer_, model_, &pointSpeaker_, offseter_.get(),
                      &tracker_, simulationProperties_.energyCollectionRules());

  std::vector<float> frequencies = simulationProperties_.frequencies();

  std::vector<std::promise<std::vector<float>>> promiseVec;
  std::vector<std::future<std::vector<float>>> futureVec;

  for (size_t index = 0; index < frequencies.size(); ++index) {
    std::promise<std::vector<float>> accumulatedPromise;
    promiseVec.push_back(accumulatedPromise);
    futureVec.push_back(accumulatedPromise.get_future());
  }

  const size_t processorsNumber = std::thread::hardware_concurrency();
  if (processorsNumber == 0) {
    throw std::invalid_argument("hardware concurrency is not supported");
  }
  size_t numberOfUsedProcessors = 0;

  std::vector<std::thread> threads;
  auto currentFrequencyIterator = frequencies.cbegin();
  auto currentPromiseIterator = promiseVec.begin();
  while (true) {
    while (numberOfUsedProcessors < processorsNumber) {
      float frequency = *currentFrequencyIterator;
      threads.push_back(
          std::thread(&Simulator::run, &simulator, std::move(frequency),
                      std::move(buildCollectors(
                          model_, simulationProperties_.numOfCollectors())),
                      &(*currentPromiseIterator)));

      ++currentFrequencyIterator;
      ++currentPromiseIterator;
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

  for (size_t index; index < frequencies.size(); ++index) {
    std::vector<float> result = futureVec[index].get();
    std::cout << "Frequency: " << frequencies[index] << ", data: [ ";
    for (const auto &energy : result) {
      std::cout << energy << ", ";
    }
    std::cout << "]" << std::endl;
  }
}
