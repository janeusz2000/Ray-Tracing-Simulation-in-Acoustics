#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

using constants::kPi;
using constants::kPopulation;
using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSource;
using objects::Object;
using objects::TriangleObj;

const core::Vec3 kVZero(0, 0, 0);

class SceneManagerTest : public ::testing::Test {
protected:
  bool performHitAtEnergyCollectors(const Ray &inputRay, RayHitData *hitData) {
    for (objects::EnergyCollector *energyCol : manager.getEnergyCollectors()) {
      const float kSkipFreq = 1000;
      if (energyCol->hitObject(inputRay, kSkipFreq, hitData)) {
        return true;
      }
    }
    return false;
  }
  // TODO: add this to constants.h instead of here
  float deg2rad(float angle) { return 2 * kPi * angle / 360; }

  SceneManager manager;
};

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
}
