#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

// Checks if |TRY_BLOCK| throws right |EXCPETION_TYPE| with exception message
// equal to const char* |MESSAGE|
// TODO: check substring only, because every little change requirers of changing
// TODO: whole test, which is not desired
#define ASSERT_EXCEPTION_MSG(TRY_BLOCK, EXCEPTION_TYPE, MESSAGE)               \
  try {                                                                        \
    { TRY_BLOCK; }                                                             \
    FAIL() << "exception '" << MESSAGE << "' not thrown at all!";              \
  } catch (const EXCEPTION_TYPE &e) {                                          \
    EXPECT_STREQ(MESSAGE, e.what())                                            \
        << " exception message is incorrect. Expected the following "          \
           "message:\n\n"                                                      \
        << MESSAGE << "\n";                                                    \
  } catch (...) {                                                              \
    FAIL() << "exception " << MESSAGE << " not thrown with expected type "     \
           << #EXCEPTION_TYPE << "'!";                                         \
  }
using constants::kPi;
using core::Ray;
using core::RayHitData;
using core::Vec3;

float deg2rad(float deg) { return 2 * constants::kPi * deg / 360; }

const int kSkipNumCollectors = 37;
const float kSkipFrequency = 1000;

class FakeModel : public ModelInterface {

public:
  explicit FakeModel(bool empty) : empty_(empty){};
  std::vector<objects::TriangleObj *> triangles() const override { return {}; }
  float height() const override { return 0; }
  float sideSize() const override { return 0; }
  bool empty() const override { return empty_; }

private:
  bool empty_;
};

class EnergyCollectorTest : public ::testing::Test {
protected:
  using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

  // performs ray hit at at given energy collectors, |hitData| is modified to
  // hold information where ray hit energyCollector. Returns true if hit
  // occurred, false when there was no hit
  [[nodiscard]] bool performHitCollector(const Collectors &energyCollectors,
                                         const Ray &ray, RayHitData *hitData) {
    bool hit = false;
    RayHitData closestHitData;
    for (const auto &collector : energyCollectors) {
      if (collector->hitObject(ray, kSkipFrequency, hitData)) {
        hit = true;
        if (hitData->time < closestHitData.time) {
          closestHitData = *hitData;
        }
      }
    }
    *hitData = closestHitData;
    return hit;
  }

  float getCollectorRadius(const Collectors &energyCollectors) const {
    return energyCollectors[0]->getRadius();
  }

  // returns value of angle between two neighborhood collectors and
  // Vec3(0, 0, 0)
  float getCollectorAngle(const int numCollector) const {
    return 2 * kPi / (numCollector + (numCollector % 2) - 2);
  }

  void printCollectors(const Collectors &energyCollectors) const {
    int i = 0;
    for (const auto &collector : energyCollectors) {
      std::cout << i++ << " " << *collector << std::endl;
    }
  }

  float getMaxZ(const Collectors &energyCollectors) {
    float maxZ = 0;
    for (const auto &collector : energyCollectors) {
      maxZ = std::max(maxZ, collector->getOrigin().z());
    }
    return maxZ;
  }

  float getCollectorPositionRadius(const Collectors &energyCollectors) {
    float maxX = 0;
    for (const auto &collector : energyCollectors) {
      maxX = std::max(maxX, collector->getOrigin().x());
    }
    return maxX;
  }
};
TEST_F(EnergyCollectorTest, ThrowExceptionWhenInvalidNumCollector) {

  const FakeModel nonEmptyModel(false);

  ASSERT_EXCEPTION_MSG(
      buildCollectors(nonEmptyModel, 38), std::invalid_argument,
      "numCollectors or numCollectors-1 has to be divisible by 4, got "
      "numCollectors = 38");

  ASSERT_EXCEPTION_MSG(buildCollectors(nonEmptyModel, 3), std::invalid_argument,
                       "numCollectors: 3 is less then 4");

  // Test case when numCollector - 1 % 4 = 0
  EXPECT_NO_THROW(buildCollectors(nonEmptyModel, 37));

  // Test case when numCollector % 4 = 0
  EXPECT_NO_THROW(buildCollectors(nonEmptyModel, 36));
}

TEST_F(EnergyCollectorTest, NotEvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);

  const int numCollectors = 37;
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(numCollectors, energyCollectors.size());

  Ray straightUp(Vec3::kVecZero, Vec3::kVecZ);
  RayHitData hitData;
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  const float collectorPositionRadius =
      getCollectorPositionRadius(energyCollectors);
  ASSERT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray straightDown(Vec3::kVecZero, -Vec3::kVecZ);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(Vec3::kVecZero, Vec3::kVecX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray alongY(Vec3::kVecZero, Vec3::kVecY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  const float collectorAngle = getCollectorAngle(energyCollectors.size());
  Ray at2Angle =
      Ray::makeRayFromSphericalCoords(Vec3::kVecZero, 0, 2 * collectorAngle);
  // getRayAtYAxisRotation(Vec3::kVecZero, 2 * collectorAngle);
  ASSERT_TRUE(performHitCollector(energyCollectors, at2Angle, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray atSixtyXY =
      Ray::makeRayFromSphericalCoords(Vec3::kVecZero, deg2rad(60), deg2rad(60));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray atSixtyXYOther = Ray::makeRayFromSphericalCoords(
      Vec3::kVecZero, deg2rad(120), deg2rad(120));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, EvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);

  const int numCollectors = 36;
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  Ray straightUp(Vec3::kVecZero, Vec3::kVecZ);
  RayHitData hitData;
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float collectorPositionRadius =
      getCollectorPositionRadius(energyCollectors);
  float collectorsMaxZ = getMaxZ(energyCollectors);
  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  // this comes from the fact, two origins of neighborhood collectors and
  // collision point are creates equilateral triangle which side is equal to
  // collector radius. Thats why collision point its just the point between two
  // collectors origin - height of the equilateral triangle.
  Vec3 refCollision(0, 0,
                    collectorsMaxZ - refCollectorRadius * std::sqrt(3) / 2);

  ASSERT_EQ(refCollision, hitData.collisionPoint());

  Ray straightDown(Vec3::kVecZero, -Vec3::kVecZ);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(Vec3::kVecZero, Vec3::kVecX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray alongY(Vec3::kVecZero, Vec3::kVecY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  const float collectorAngle = getCollectorAngle(energyCollectors.size());
  Ray atAngle =
      Ray::makeRayFromSphericalCoords(Vec3::kVecZero, 0, collectorAngle);
  ASSERT_TRUE(performHitCollector(energyCollectors, atAngle, &hitData));
  ASSERT_FLOAT_EQ(collectorPositionRadius - refCollectorRadius, hitData.time);

  Ray at30XY =
      Ray::makeRayFromSphericalCoords(Vec3::kVecZero, deg2rad(30), deg2rad(60));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray at30XYOther(Vec3::kVecZero,
                  Vec3(-std::cos(deg2rad(30)), -std::cos(deg2rad(30)),
                       std::sin(deg2rad(60))));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, NoHoleNextToTheTopCollectorOddNum) {

  const FakeModel nonEmptyModel(false);
  const int numCollectors = 37;

  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  const float collectorPositionRadius = 4;
  // this is how previous implementation was caclualating radius of energy
  // collector
  float invalidEnergyCollectorRadius =
      2 * kPi * collectorPositionRadius / numCollectors;

  RayHitData hitData;
  Ray previousNotHit1(
      Vec3::kVecZero,
      Vec3(0, 1.01 * invalidEnergyCollectorRadius, collectorPositionRadius));
  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit1, &hitData));

  Ray previousNotHit2(
      Vec3::kVecZero,
      Vec3(0, -1.01 * invalidEnergyCollectorRadius, collectorPositionRadius));

  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit2, &hitData));
}

TEST_F(EnergyCollectorTest, HitRayStraightUpEvenCollectors) {

  const FakeModel nonEmptyModel(false);
  const int numCollectors = 20;

  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  RayHitData hitData;
  Ray straightUp(Vec3::kVecZero, Vec3::kVecZ);
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  const float collectorPositionRadius = 4;
  const float refCollectorRadius = getCollectorRadius(energyCollectors);
  float collectorsMaxZ = getMaxZ(energyCollectors);
  // See EvenNumOfEnergyCollectorTest for explanation
  ASSERT_FLOAT_EQ(collectorsMaxZ - refCollectorRadius * std::sqrt(3) / 2,
                  hitData.time);
}