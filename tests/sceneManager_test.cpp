#include "main/model.h"
#include "main/sceneManager.h"
#include "main/trackers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>

using collectionRules::LinearEnergyCollection;
using trackers::CollectorsTrackerInterface;
using trackers::PositionTrackerInterface;

using Energies = std::vector<float>;
using EnergiesPerFrequency = std::vector<Energies>;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

const float kSkipFreq = 1000;

class FakePositionTracker : public PositionTrackerInterface {
public:
  void initializeNewTracking() override{};
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override{};
  void endCurrentTracking() override{};
  void clearTracking() override{};
  void save() const override{};
};

struct FakeCollectorsTracker : public CollectorsTrackerInterface {
  void save(const Collectors &collectors, std::string_view path) override{};
};

class SceneManagerSimpleTest : public ::testing::Test {
public:
  SceneManagerSimpleTest() {
    float modelSize = 1.0; // [m]
    model = Model::NewReferenceModel(modelSize);
  };

protected:
  std::unique_ptr<Model> model;
  FakePositionTracker positionTracker;
  FakeCollectorsTracker collectorsTracker;
  LinearEnergyCollection energyCollectionRules;
};

TEST_F(SceneManagerSimpleTest, repetitiveCollectionOfEnergyTest) {
  float sourcePower = 500;
  int numOfCollectors = 37;
  int numOfRaysSquared = 1;

  std::vector<float> frequencies = {kSkipFreq, kSkipFreq};

  SimulationProperties simulationProperties(frequencies, &energyCollectionRules,
                                            sourcePower, numOfCollectors,
                                            numOfRaysSquared);

  SceneManager singleRaySimulation(model.get(), simulationProperties,
                                   &positionTracker, &collectorsTracker);
  EnergiesPerFrequency result = singleRaySimulation.run();
  ASSERT_THAT(result[0], result[1]);
}