#include "constants.h"
#include "main/simulator.h"
#include "gmock/gmock.h"
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

class MockModel : public AbstractModel {

public:
  explicit MockModel(bool empty) : empty_(empty){};
  MOCK_METHOD(std::vector<objects::TriangleObj *>, triangles, (),
              (const override));
  MOCK_METHOD(float, height, (), (const override));
  MOCK_METHOD(float, sideSize, (), (const override));
  // TODO: how to return values with gMock model
  MOCK_METHOD(bool, empty, (), (const override));

private:
  bool empty_;
};

class EnergyCollectorTest : public ::testing::Test {
public:
  enum class HitResult { ENERGY_COLLECTORS_EMPTY, NO_HIT, HIT };

protected:
  // performs ray hit at given energyCollectors by modifying hitData.
  // Returns:
  // ENERGY_COLLECTORS_EMPTY - when no energyCollectors where assigned
  // NO_HIT - when there was no hit
  // HIT - when hit occurred;
  [[nodiscard]] HitResult performHitCollector(
      const Ray &ray,
      std::vector<std::unique_ptr<objects::EnergyCollector>> *energyCollectors,
      RayHitData *hitData) {

    if (energyCollectors->empty()) {
      return HitResult::ENERGY_COLLECTORS_EMPTY;
    }

    for (const auto &collector : *energyCollectors) {
      if (collector->hitObject(ray, kSkipFrequency, hitData)) {
        return HitResult::HIT;
      }
    }

    return HitResult::NO_HIT;
  }

  void printCollectors(std::vector<std::unique_ptr<objects::EnergyCollector>>
                           *energyCollectors) const {
    for (const auto &collector : *energyCollectors) {
      std::cout << "Origin: " << collector->getOrigin() << "\n"
                << "Radius: " << collector->getRadius() << "\n";
    }
  }
};
TEST_F(EnergyCollectorTest, ThrowExceptionWhenInvalidNumCollector) {

  MockModel nonEmptyModel(false);

  const int invalidNumCollector = 38;
  ASSERT_THROW(buildCollectors(nonEmptyModel, invalidNumCollector),
               std::invalid_argument);

  // Test case when numCollector is less then 1:
  const int numCollectorLessThenOne = 0;
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
  MockModel nonEmptyModel(false);

  // This value will be used in actual simulation
  const int numCollector = 37;
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollector);

  // TODO: THis should be done inside class fixture
  // Because fake model height and size are equal to 0,
  // collectors origin distance to Vec3(0, 0, 0) is always equal to 4.
  const float collectorPositionRadius = 4;
  const float collectorAngle = 2 * kPi / (numCollector - 1);
  const float refCollectorRadius =
      collectorPositionRadius * std::sqrt(2 - 2 * std::cos(collectorAngle));

  ASSERT_EQ(energyCollectors.size(), numCollector);

  Ray straightUp(kVecZero, kVecUp);
  RayHitData hitData;
  ASSERT_EQ(performHitCollector(straightUp, &energyCollectors, &hitData),
            HitResult::HIT);

  Vec3 collisionPointStraightUp =
      Vec3(0, 0, collectorPositionRadius - refCollectorRadius);
  ASSERT_EQ(hitData.collisionPoint(), collisionPointStraightUp);

  Ray straightDown(kVecZero, -kVecUp);
  ASSERT_EQ(performHitCollector(straightDown, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(kVecZero, kVecX);
  ASSERT_EQ(performHitCollector(alongX, &energyCollectors, &hitData),
            HitResult::HIT);

  ASSERT_EQ(alongX.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray alongY(kVecZero, kVecY);
  ASSERT_EQ(performHitCollector(alongY, &energyCollectors, &hitData),
            HitResult::HIT);

  ASSERT_EQ(alongY.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray atSixty(kVecZero, Vec3(std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixty, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray atSixtyOther(kVecZero,
                   Vec3(-std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixtyOther, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray atSixtyXY(kVecZero, Vec3(std::cos(deg2rad(60)), std::cos(deg2rad(60)),
                               std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixtyXY, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();

  Ray atSixtyXYOther(kVecZero,
                     Vec3(-std::cos(deg2rad(60)), -std::cos(deg2rad(60)),
                          std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixtyXYOther, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, EvenNumOfEnergyCollectorTest) {
  const int numCollectors = 36;
  const float collectorPositionRadius = 4;
  const float collectorAngle = 2 * kPi / (numCollectors - 2);
  const float refCollectorRadius =
      collectorPositionRadius * std::sqrt(2 - 2 * std::cos(collectorAngle));

  MockModel nonEmptyModel(false);
  auto energyCollectors = buildCollectors(nonEmptyModel, numCollectors);

  ASSERT_EQ(energyCollectors.size(), numCollectors);

  Ray straightUp(kVecZero, kVecUp);
  RayHitData hitData;
  ASSERT_EQ(performHitCollector(straightUp, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray straightDown(kVecZero, -kVecUp);
  ASSERT_EQ(performHitCollector(straightDown, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(kVecZero, kVecX);
  ASSERT_EQ(performHitCollector(alongX, &energyCollectors, &hitData),
            HitResult::HIT);

  ASSERT_EQ(alongX.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray alongY(kVecZero, kVecY);
  ASSERT_EQ(performHitCollector(alongY, &energyCollectors, &hitData),
            HitResult::HIT);

  ASSERT_EQ(alongY.at(collectorPositionRadius - refCollectorRadius),
            hitData.collisionPoint());

  Ray at30(kVecZero, Vec3(std::cos(deg2rad(30)), 0, std::sin(deg2rad(30))));
  ASSERT_EQ(performHitCollector(at30, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray at30other(kVecZero,
                Vec3(-std::cos(deg2rad(30)), 0, std::sin(deg2rad(30))));
  ASSERT_EQ(performHitCollector(at30other, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray at30XY(kVecZero, Vec3(std::cos(deg2rad(30)), std::cos(deg2rad(30)),
                            std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(at30XY, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();

  Ray at30XYOther(kVecZero, Vec3(-std::cos(deg2rad(30)), -std::cos(deg2rad(30)),
                                 std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(at30XYOther, &energyCollectors, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, PositionsThatWereFixedTest) {
  const int numCollector = 37;
  const float collectorPositionRadius = 4;

  MockModel nonEmptyModel(false);

  auto energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  ASSERT_EQ(energyCollectors.size(), numCollector);

  // this is how previous implementation was caclualating radius of energy
  // collector
  float invalidEnergyCollectorRadius =
      2 * kPi * collectorPositionRadius / static_cast<float>(numCollector);

  RayHitData hitData;
  Ray previousNotHit1(kVecZero,
                      Vec3(0, 1.01 * invalidEnergyCollectorRadius, 1));
  ASSERT_EQ(performHitCollector(previousNotHit1, &energyCollectors, &hitData),
            HitResult::HIT);

  Ray previousNotHit2(kVecZero,
                      Vec3(0, -1.01 * invalidEnergyCollectorRadius, 1));

  ASSERT_EQ(performHitCollector(previousNotHit2, &energyCollectors, &hitData),
            HitResult::HIT);
}
