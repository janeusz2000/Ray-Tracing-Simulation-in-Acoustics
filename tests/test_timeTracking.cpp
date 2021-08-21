#include "main/model.h"
#include "main/rayTracer.h"
#include "main/resultsCalculation.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "main/trackers.h"
#include "obj/generators.h"

#include "gtest/gtest.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using EnergyPerTime = std::unordered_map<float, float>;
using Energy = std::vector<EnergyPerTime>;
using EnergyPerFrequency = std::unordered_map<float, Energy>;
using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

class BasicSimulationTest : public ::testing::Test {
public:
  explicit BasicSimulationTest()
      : simplePlainSquareModel(Model::NewReferenceModel(/*modelSize=*/1.0f)),
        basicProperties(/*frequencies=*/{1000},
                        /*sourcePower=*/500,
                        /*numOfCollectors=*/37,
                        /*numOfRaysSquared=*/1,
                        /*maxTracking=*/1),
        properties(&energyCollectionRules, basicProperties),
        rayTracer(simplePlainSquareModel.get()),
        rayFactory(basicProperties.numOfRaysSquared,
                   basicProperties.sourcePower, simplePlainSquareModel.get()) {}

protected:
  Simulator getBasicSimulator();
  Collectors buildCollectorsAroundModel() const;

  std::unique_ptr<Model> simplePlainSquareModel;

  trackers::FakePositionTracker positionTracker;
  trackers::FakeCollectorsTracker collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties;
  SimulationProperties properties;

  RayTracer rayTracer;
  FakeReflectionEngine reflectionEngine;

  generators::FakeOffseter rayOffseter;
  generators::PointSpeakerRayFactory rayFactory;

  DoubleAxisCollectorBuilder collectorBuilder;

private:
  const int kSampleRate_ = 96e3;
};

Simulator BasicSimulationTest::getBasicSimulator() {
  rayFactory = generators::PointSpeakerRayFactory(
      basicProperties.numOfRaysSquared, basicProperties.sourcePower,
      simplePlainSquareModel.get());
  return Simulator(&rayTracer, simplePlainSquareModel.get(), &rayFactory,
                   &rayOffseter, &positionTracker, &energyCollectionRules,
                   &reflectionEngine);
}

Collectors BasicSimulationTest::buildCollectorsAroundModel() const {
  return collectorBuilder.buildCollectors(simplePlainSquareModel.get(),
                                          basicProperties.numOfCollectors);
}

TEST_F(BasicSimulationTest, AccumulatedTimeSingleRay) {
  Simulator basicSimulator = getBasicSimulator();
  Collectors collectors = buildCollectorsAroundModel();

  const int maxTemporaryTracking = 2;

  for (float frequency : basicProperties.frequencies) {
    int currentTracking = 0;
    core::Ray singleRay;
    while (rayFactory.genRay(&singleRay)) {
      basicSimulator.performRayTracing(&collectors, frequency, &singleRay,
                                       maxTemporaryTracking, &currentTracking);
    }
    ASSERT_EQ(currentTracking, 1);
  }
}
