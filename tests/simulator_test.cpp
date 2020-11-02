#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

using constants::kDefaultEnergyCollectorRadius;
using constants::kDefaultSimulationRadius;
using constants::kSkipFreq;
using constants::kSkipPath;
using core::Ray;
using core::RayHitData;
using core::Vec3;

const Vec3 kDefaultSourcePosition(0, 0, constants::kDefaultSimulationRadius);
const Vec3 kVecStraightDown(0, 0, -1);
const float kDefaultEnergyPerRay =
    constants::kDefaultSourcePower /
    (constants::kDefaultNumRaysPerRow * constants::kDefaultNumRaysPerRow);

class ReferencePlateTest : public ::testing::Test {
protected:
  Simulator sim;
};

TEST_F(ReferencePlateTest, EnergyCollectionFromRay) {

  RayHitData hitData;

  Ray starightDown(kDefaultSourcePosition, kVecStraightDown,
                   kDefaultEnergyPerRay);
  sim.rayTrace(starightDown, &hitData, 2);
  ASSERT_EQ(hitData.collisionPoint(), Vec3(0, 0, kDefaultSimulationRadius / 2));

  ASSERT_EQ(hitData.energy(), kDefaultEnergyPerRay);
}
