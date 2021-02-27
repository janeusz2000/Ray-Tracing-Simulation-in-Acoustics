#include "main/model.h"
#include "main/sceneManager.h"
#include "main/trackers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>
#include <unordered_map>
#include <vector>

using collectionRules::LinearEnergyCollection;
using trackers::CollectorsTrackerInterface;
using trackers::PositionTrackerInterface;

using Energies = std::vector<float>;
using EnergiesPerFrequency = std::unordered_map<float, Energies>;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

const float kSkipFreq = 1000;

class FakePositionTracker : public PositionTrackerInterface {
public:
  void initializeNewFrequency(float frequency) override{};
  void initializeNewTracking() override{};
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override{};
  void endCurrentFrequency() override{};
  void endCurrentTracking() override{};
  void save() override{};

private:
  void printItself(std::ostream &os) const noexcept override {
    os << "Fake Position Tracker";
  }
};

struct FakeCollectorsTracker : public CollectorsTrackerInterface {
  void save(const Collectors &collectors, std::string_view path) override{};

private:
  void printItself(std::ostream &os) const noexcept override {
    os << "Fake Collectors Tracker";
  }
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

  std::vector<float> frequencies = {100, 200};

  BasicSimulationProperties basicProperties(frequencies, sourcePower,
                                            numOfCollectors, numOfRaysSquared);
  SimulationProperties simulationProperties(&energyCollectionRules,
                                            basicProperties);

  SceneManager singleRaySimulation(model.get(), simulationProperties,
                                   &positionTracker, &collectorsTracker);
  EnergiesPerFrequency result = singleRaySimulation.run();
  // ! THIS DOESNT TEST ANYTHING, FIX IT!
  ASSERT_THAT(result[100], ::testing::Not(::testing::IsEmpty()));
  ASSERT_EQ(result[100], result[200]);
}