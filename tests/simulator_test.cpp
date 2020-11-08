#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

using constants::kPi;
using core::Ray;
using core::RayHitData;
using core::Vec3;

const int kSkipNumCollectors = 37;
const float kSkipFrequency = 1000;
const Vec3 kVecZero(0, 0, 0);
const Vec3 kVecUp(0, 0, 1);
const Vec3 kVecX(1, 0, 0);
const Vec3 kVecY(0, 1, 0);

float deg2rad(float deg) { return 2 * kPi * deg / 360; }

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
  // performs ray hit at at given energy collectors, |hitData| is modified to
  // hold information where ray hit energyCollector. Returns true if hit
  // occurred, false when there was no hit
  [[nodiscard]] bool performHitCollector(
      const std::vector<std::unique_ptr<objects::EnergyCollector>>
          &energyCollectors,
      const Ray &ray, RayHitData *hitData) {

    for (const auto &collector : energyCollectors) {
      if (collector->hitObject(ray, kSkipFrequency, hitData)) {
        return true;
      }
    }

    return false;
  }

  void printCollectors(std::vector<std::unique_ptr<objects::EnergyCollector>>
                           energyCollectors) const {
    for (const auto &collector : energyCollectors) {
      std::cout << "Origin: " << collector->getOrigin() << "\n"
                << "Radius: " << collector->getRadius() << "\n";
    }
  }
};
TEST_F(EnergyCollectorTest, ThrowExceptionWhenInvalidNumCollector) {

  const FakeModel nonEmptyModel(false);

  const int invalidNumCollectors = 38;
  ASSERT_THROW(buildCollectors(nonEmptyModel, invalidNumCollectors),
               std::invalid_argument);

  const int numCollectorLessThenOne = 3;
  ASSERT_THROW(buildCollectors(nonEmptyModel, numCollectorLessThenOne),
               std::invalid_argument);

  // Test case when numCollector - 1 % 4 = 0
  const int validNumCollectorCase1 = 37;
  ASSERT_NO_THROW(buildCollectors(nonEmptyModel, validNumCollectorCase1));

  // Test case when numCollector % 4 = 0
  const int validNumCollectorCase2 = 36;
  ASSERT_NO_THROW(buildCollectors(nonEmptyModel, validNumCollectorCase2));
}

TEST_F(EnergyCollectorTest, NotEvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);

  const int numCollectors = 37;
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(numCollectors, energyCollectors.size());

  Ray straightUp(kVecZero, kVecUp);
  RayHitData hitData;
  ASSERT_EQ(performHitCollector(energyCollectors, straightUp, &hitData), true);

  const float collectorPositionRadius = 4;
  const float collectorAngle = 2 * kPi / (numCollectors - 1);
  const float refCollectorRadius =
      collectorPositionRadius * std::sqrt(2 - 2 * std::cos(collectorAngle));

  Vec3 collisionPointStraightUp =
      Vec3(0, 0, collectorPositionRadius - refCollectorRadius);
  ASSERT_EQ(hitData.collisionPoint(), collisionPointStraightUp);

  Ray straightDown(kVecZero, -kVecUp);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(kVecZero, kVecX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));

  ASSERT_EQ(alongX.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray alongY(kVecZero, kVecY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));

  ASSERT_EQ(alongY.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray atSixty(kVecZero, Vec3(std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_TRUE(performHitCollector(energyCollectors, atSixty, &hitData));

  Ray atSixtyOther(kVecZero,
                   Vec3(-std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_TRUE(performHitCollector(energyCollectors, atSixtyOther, &hitData));

  Ray atSixtyXY(kVecZero, Vec3(std::cos(deg2rad(60)), std::cos(deg2rad(60)),
                               std::sin(deg2rad(60))));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray atSixtyXYOther(kVecZero,
                     Vec3(-std::cos(deg2rad(60)), -std::cos(deg2rad(60)),
                          std::sin(deg2rad(60))));
  ASSERT_FALSE(performHitCollector(energyCollectors, atSixtyXYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, EvenNumOfEnergyCollectorTest) {
  const FakeModel nonEmptyModel(false);

  const int numCollectors = 36;
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  Ray straightUp(kVecZero, kVecUp);
  RayHitData hitData;
  ASSERT_TRUE(performHitCollector(energyCollectors, straightUp, &hitData));

  Ray straightDown(kVecZero, -kVecUp);
  ASSERT_FALSE(performHitCollector(energyCollectors, straightDown, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(kVecZero, kVecX);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongX, &hitData));

  const float collectorPositionRadius = 4;
  const float collectorAngle = 2 * kPi / (numCollectors - 2);
  const float refCollectorRadius =
      collectorPositionRadius * std::sqrt(2 - 2 * std::cos(collectorAngle));
  ASSERT_EQ(alongX.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray alongY(kVecZero, kVecY);
  ASSERT_TRUE(performHitCollector(energyCollectors, alongY, &hitData));

  ASSERT_EQ(alongY.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray at30(kVecZero, Vec3(std::cos(deg2rad(30)), 0, std::sin(deg2rad(30))));
  ASSERT_TRUE(performHitCollector(energyCollectors, at30, &hitData));

  Ray at30other(kVecZero,
                Vec3(-std::cos(deg2rad(30)), 0, std::sin(deg2rad(30))));
  ASSERT_TRUE(performHitCollector(energyCollectors, at30other, &hitData));

  Ray at30XY(kVecZero, Vec3(std::cos(deg2rad(30)), std::cos(deg2rad(30)),
                            std::sin(deg2rad(60))));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XY, &hitData))
      << "Collision Point: " << hitData.collisionPoint();

  Ray at30XYOther(kVecZero, Vec3(-std::cos(deg2rad(30)), -std::cos(deg2rad(30)),
                                 std::sin(deg2rad(60))));
  ASSERT_FALSE(performHitCollector(energyCollectors, at30XYOther, &hitData))
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, PositionsThatWereFixedTest) {

  const FakeModel nonEmptyModel(false);
  const int numCollectors = 37;

  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);
  ASSERT_EQ(energyCollectors.size(), numCollectors);

  const float collectorPositionRadius = 4;
  // this is how previous implementation was caclualating radius of energy
  // collector
  float invalidEnergyCollectorRadius =
      2 * kPi * collectorPositionRadius / static_cast<float>(numCollectors);

  RayHitData hitData;
  Ray previousNotHit1(kVecZero,
                      Vec3(0, 1.01 * invalidEnergyCollectorRadius, 1));
  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit1, &hitData));

  Ray previousNotHit2(kVecZero,
                      Vec3(0, -1.01 * invalidEnergyCollectorRadius, 1));

  ASSERT_TRUE(performHitCollector(energyCollectors, previousNotHit2, &hitData));
}
