#include "main/model.h"
#include "main/rayTracer.h"
#include "main/resultsCalculation.h"
#include "main/sceneManager.h"
#include "main/simulator.h"
#include "main/trackers.h"
#include "obj/generators.h"

#include "gtest/gtest.h"

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define ASSERT_OPTIONAL_NOT_EMPTY(OPTIONAL_TYPE)                               \
  {                                                                            \
    if (!OPTIONAL_TYPE.has_value()) {                                          \
      FAIL() << "Given Optional: " << #OPTIONAL_TYPE << " "                    \
             << "was empty!";                                                  \
    }                                                                          \
    SUCCEED();                                                                 \
  }

using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

float convertDistanceIntoTravelTime(float distance) {
  return distance / constants::kSoundSpeed;
}

std::unique_ptr<Model>
getInfiniteLoopRayTracingModel(float distanceBetweenPlates, float size) {
  auto getClockWiseOrigins = [&](float size) -> std::vector<core::Vec3> {
    return {core::Vec3(-size, size, 0), core::Vec3(size, size, 0),
            core::Vec3(size, -size, 0), core::Vec3(-size, -size, 0)};
  };
  auto getTriangles = [&](std::vector<core::Vec3> &clockWiseOrigins)
      -> std::vector<objects::TriangleObj> {
    return {objects::TriangleObj(clockWiseOrigins[1], clockWiseOrigins[2],
                                 clockWiseOrigins[3]),
            objects::TriangleObj(clockWiseOrigins[3], clockWiseOrigins[0],
                                 clockWiseOrigins[1])};
  };

  std::vector<core::Vec3> firstLayer = getClockWiseOrigins(size);
  std::vector<objects::TriangleObj> firstLayerTriangles =
      getTriangles(firstLayer);

  std::vector<core::Vec3> secondLayer = getClockWiseOrigins(size);
  std::for_each(secondLayer.begin(), secondLayer.end(),
                [&](core::Vec3 &position) {
                  position += distanceBetweenPlates * core::Vec3::kZ;
                });

  std::vector<objects::TriangleObj> secondLayerTriangles =
      getTriangles(secondLayer);

  firstLayerTriangles.insert(firstLayerTriangles.end(),
                             secondLayerTriangles.begin(),
                             secondLayerTriangles.end());
  return std::make_unique<Model>(firstLayerTriangles);
};

class TestPositionTracker : public trackers::PositionTrackerInterface {
public:
  void
  addNewPositionToCurrentTracking(const core::RayHitData &hitData) override;
  void printItself(std::ostream &os) const noexcept override;

  std::optional<core::RayHitData> getLastReachedPosition() const;
  size_t getNumberOfAcquiredTrackings() const;

private:
  std::vector<core::RayHitData> reachedPositions_;

public:
  // not important
  void initializeNewFrequency(float frequency) override{};
  void initializeNewTracking() override{};
  void endCurrentFrequency() override{};
  void endCurrentTracking() override{};
  void save() override{};
  void switchToReferenceModel() override{};
};

void TestPositionTracker::addNewPositionToCurrentTracking(
    const core::RayHitData &hitData) {
  reachedPositions_.push_back(hitData);
}

void TestPositionTracker::printItself(std::ostream &os) const noexcept {
  os << "Test Position tracker\n"
     << "\tReached positions:\n";
  for (const core::RayHitData &hitData : reachedPositions_) {
    os << hitData;
  };
}

std::optional<core::RayHitData>
TestPositionTracker::getLastReachedPosition() const {
  return reachedPositions_.size() > 0
             ? std::make_optional(reachedPositions_.back())
             : std::nullopt;
}

size_t TestPositionTracker::getNumberOfAcquiredTrackings() const {
  return reachedPositions_.size();
}

class BasicSimulationTest : public ::testing::Test {
public:
  explicit BasicSimulationTest()
      : simplePlainSquareModel(Model::NewReferenceModel(/*modelSize=*/1.0f)),
        basicProperties(/*frequencies=*/{1000},
                        /*sourcePower=*/500,
                        /*numOfCollectors=*/37,
                        /*numOfRaysSquared=*/1,
                        /*maxTracking=*/15),
        properties(&energyCollectionRules, basicProperties),
        rayTracer(simplePlainSquareModel.get()),
        rayFactory(basicProperties.numOfRaysSquared,
                   basicProperties.sourcePower, simplePlainSquareModel.get()) {}

protected:
  Simulator getBasicSimulator();
  Simulator getBasicSimulatorWithCustomRayFactory(
      generators::PointSpeakerRayFactory *customRayFactory);
  Collectors buildCollectorsAroundModel() const;
  void performRayTracing(Simulator *simulator, Collectors *collectors,
                         int maxTracking);

  std::unique_ptr<Model> simplePlainSquareModel;

  TestPositionTracker positionTracker;
  trackers::FakeCollectorsTracker collectorsTracker;

  collectionRules::NonLinearEnergyCollection energyCollectionRules;
  BasicSimulationProperties basicProperties;
  SimulationProperties properties;

  RayTracer rayTracer;
  FakeReflectionEngine reflectionEngine;

  generators::FakeOffseter rayOffseter;
  generators::PointSpeakerRayFactory rayFactory;

  DoubleAxisCollectorBuilder collectorBuilder;
};

Simulator BasicSimulationTest::getBasicSimulator() {
  // reseting all necessary data before each simulation
  rayFactory = generators::PointSpeakerRayFactory(
      basicProperties.numOfRaysSquared, basicProperties.sourcePower,
      simplePlainSquareModel.get());
  positionTracker = TestPositionTracker();
  return Simulator(&rayTracer, simplePlainSquareModel.get(), &rayFactory,
                   &rayOffseter, &positionTracker, &energyCollectionRules,
                   &reflectionEngine);
}

Collectors BasicSimulationTest::buildCollectorsAroundModel() const {
  return collectorBuilder.buildCollectors(simplePlainSquareModel.get(),
                                          basicProperties.numOfCollectors);
}

void BasicSimulationTest::performRayTracing(Simulator *simulator,
                                            Collectors *collectors,
                                            int maxTracking) {
  for (float frequency : basicProperties.frequencies) {
    int currentTracking = 0;
    core::Ray singleRay;
    while (rayFactory.genRay(&singleRay)) {
      simulator->performRayTracing(collectors, frequency, &singleRay,
                                   maxTracking, &currentTracking);
    }
  }
}

TEST_F(BasicSimulationTest, AccumulatedTimeSingleRay) {
  Simulator basicSimulator = getBasicSimulator();
  Collectors collectors = buildCollectorsAroundModel();

  performRayTracing(&basicSimulator, &collectors, basicProperties.maxTracking);

  int expectedRayTrackings = 2;
  ASSERT_EQ(expectedRayTrackings,
            positionTracker.getNumberOfAcquiredTrackings());

  std::optional<core::RayHitData> lastHitData =
      positionTracker.getLastReachedPosition();
  ASSERT_OPTIONAL_NOT_EMPTY(lastHitData);
  float traveledDistance = 1.5 * rayFactory.origin().magnitude();
  float expectedTravelTime = convertDistanceIntoTravelTime(traveledDistance);
  ASSERT_FLOAT_EQ(expectedTravelTime, lastHitData->accumulatedTime);
}

TEST_F(BasicSimulationTest, InfiniteLoopAccumulatedTimeTest) {
  // Collectors collectors = buildCollectorsAroundModel();
  // const float distanceBetweenTwoPlates = 2; // [m]
  // const float size = 1;
  // std::unique_ptr<Model> model =
  //     getInfiniteLoopRayTracingModel(distanceBetweenTwoPlates, size);

  // generators::CustomPointRayFactory customRayFactory(
  //     /*origin=*/core::Vec3(0, 0, 2),
  //     /*direction=*/core::Vec3(0, 0, -1),
  //     /*energy=*/500);

  // RayTracer customRayTracer(model.get());
  // positionTracker = TestPositionTracker();
  // Simulator simulator(&customRayTracer, model.get(), &customRayFactory,
  //                     &rayOffseter, &positionTracker, &energyCollectionRules,
  //                     &reflectionEngine);
  // objects::SphereWall sphereWall(/*radius=*/32);
  // simulator.setSphereWall(sphereWall);

  // int maxTracking = 4;
  // performRayTracing(&simulator, &collectors, maxTracking);
  // model->printTriangles();
  // std::cout << simulator << std::endl;
  // FAIL() << "THIS TEST WAS SUPPOSED TO FAIL";
  // int maxTracking = 8;
  // performRayTracing(&basicSimulator, &collectors, maxTracking);
  // int expectedTrackings = 4;
  // ASSERT_EQ(expectedTrackings,
  //           positionTracker.getNumberOfAcquiredTrackings()); //
}
