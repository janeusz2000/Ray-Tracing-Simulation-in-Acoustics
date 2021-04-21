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

using EnergyPerTime = std::unordered_map<float, float>;
using Energies = std::vector<EnergyPerTime>;
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
  void switchToReferenceModel() override{};

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
