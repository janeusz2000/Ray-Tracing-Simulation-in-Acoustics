#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

const int kSkipNumCollectors = 37;

class FakeModel : public AbstractModel {

public:
  explicit FakeModel(bool empty) : empty_(empty){};
  std::vector<objects::TriangleObj *> triangles() const override { return {}; }
  float height() const override { return 0; }
  float sideSize() const override { return 0; }
  bool empty() const { return empty_; }

private:
  bool empty_;
};

class EnergyCollectorTest : public ::testing::Test {
public:
  EnergyCollectorTest() : nonEmptyModel(false), emptyModel(true){};

protected:
  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors;
  FakeModel nonEmptyModel, emptyModel;
};

TEST_F(EnergyCollectorTest, ThrowingExceptionWhenEmpty) {
  ASSERT_THROW(buildCollectors(emptyModel, kSkipNumCollectors),
               std::invalid_argument);
  ASSERT_NO_THROW(buildCollectors(nonEmptyModel, kSkipNumCollectors));
}

TEST_F(EnergyCollectorTest, ThrowExceptionWhenInvalidNumCollector) {
  const int invalidNumCollector = 38;
  ASSERT_THROW(buildCollectors(nonEmptyModel, invalidNumCollector),
               std::invalid_argument);

  // Test case when numCollector is less then 1:
  const int numCollectorLessThenZero = 0;
  ASSERT_THROW(buildCollectors(nonEmptyModel, numCollectorLessThenZero),
               std::invalid_argument);

  // Test case when numCollector - 1 % 4 = 0
  const int validNumCollectorCase1 = 37;
  ASSERT_NO_THROW(buildCollectors(nonEmptyModel, validNumCollectorCase1));

  // Test case when numCollector % 4 = 0
  const int validNumCollectorCase2 = 36;
  ASSERT_NO_THROW(buildCollectors(nonEmptyModel, validNumCollectorCase2));
}

// ! OLD ENERGY COLLECTORS TEST
/*
TEST_F(SceneManagerTest, EnergyCollectorPositionsCheck) {
  for (const auto &collectorPtr : manager.getEnergyCollectors()) {
    std::cout << *collectorPtr << std::endl;
  }

  ASSERT_EQ(manager.getEnergyCollectors().size(), kPopulation);

  RayHitData hitData;
  Ray straightUp(kVZero, core::Vec3(0, 0, 1));

  ASSERT_TRUE(performHitAtEnergyCollectors(straightUp, &hitData));

  core::Vec3 referenceCollisionPoint(
      0, 0, manager.simulatiorRadius() / 2 - manager.collectorRadius());
  ASSERT_EQ(hitData.collisionPoint(), referenceCollisionPoint)
      << "point from: " << straightUp;

  Ray alongXAxis(kVZero, core::Vec3(-1, 0, 0));
  ASSERT_TRUE(performHitAtEnergyCollectors(alongXAxis, &hitData));

  Vec3 referenceCollisionPoint2(
      -(manager.simulatiorRadius() / 2 - manager.collectorRadius()), 0, 0);
  ASSERT_EQ(hitData.collisionPoint(), referenceCollisionPoint2)
      << " point from: " << alongXAxis;

  Ray alongYAxis(kVZero, core::Vec3(0, 1, 0));
  ASSERT_TRUE(performHitAtEnergyCollectors(alongYAxis, &hitData));

  Vec3 referenceCollisionPoint3(
      0, manager.simulatiorRadius() / 2 - manager.collectorRadius(), 0);
  ASSERT_EQ(hitData.collisionPoint(), referenceCollisionPoint3)
      << " point from: " << alongYAxis;

  Ray atSixtyDegreesAlongX(
      kVZero, core::Vec3(std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_TRUE(performHitAtEnergyCollectors(atSixtyDegreesAlongX, &hitData));

  Vec3 referenceCollisionAtSixty(
      manager.simulatiorRadius() / 2 * std::cos(deg2rad(60)) -
          manager.collectorRadius() * std::cos(deg2rad(60)),
      0,
      manager.simulatiorRadius() / 2 * std::sin(deg2rad(60)) -
          manager.collectorRadius() * std::sin(deg2rad(60)));

  ASSERT_EQ(hitData.collisionPoint(), referenceCollisionAtSixty)
      << "Invalid hit from " << atSixtyDegreesAlongX;

  Ray straightDown(kVZero, core::Vec3(0, 0, -1));
  ASSERT_FALSE(performHitAtEnergyCollectors(straightDown, &hitData))
      << "hit registered in: " << hitData.collisionPoint();

  // Ray at the edge of EnergyCollector at: Vec3(0, 0, 2)
  Ray straightUpAbove(
      core::Vec3(0, 0,
                 manager.simulatiorRadius() / 2 + manager.collectorRadius()),
      core::Vec3(0, 0, 1));
  ASSERT_FALSE(performHitAtEnergyCollectors(straightUpAbove, &hitData))
      << "hit registered in: " << hitData.collisionPoint();

  Ray insideEnergyCollectorStraightUp(
      core::Vec3(manager.simulatiorRadius() / 2, 0, 0), core::Vec3(0, 0, 1));
  ASSERT_TRUE(
      performHitAtEnergyCollectors(insideEnergyCollectorStraightUp, &hitData));

  ASSERT_EQ(
      core::Vec3(manager.simulatiorRadius() / 2, 0, manager.collectorRadius()),
      hitData.collisionPoint());

  Ray tiltedFromXY(kVZero, Vec3(2 * manager.collectorRadius(),
                                2 * manager.collectorRadius(), 2));
  ASSERT_FALSE(performHitAtEnergyCollectors(tiltedFromXY, &hitData));

  Ray straightUpMoved(
      Vec3(2 * manager.collectorRadius(), 2 * manager.collectorRadius(), 0),
      Vec3(0, 0, 1));
  ASSERT_FALSE(performHitAtEnergyCollectors(straightUpMoved, &hitData));
} */