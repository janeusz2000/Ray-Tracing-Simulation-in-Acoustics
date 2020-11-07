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

float deg2rad(float deg) { return 2 * kPi * deg / 360; }

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
  enum class HitResult { ENERGY_COLLECTORS_EMPTY, NO_HIT, HIT };

protected:
  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors;
  FakeModel nonEmptyModel, emptyModel;

  // performs ray hit at energy Collectors by modifying hitData.
  // Returns:
  // ENERGY_COLLECTORS_EMPTY - when no energyCollectors where assigned
  // NO_HIT - when there was no hit
  // HIT - when hit occurred;
  [[nodiscard]] HitResult performHitCollector(const Ray &ray, float frequency,
                                              RayHitData *hitData) {

    if (energyCollectors.empty()) {
      return HitResult::ENERGY_COLLECTORS_EMPTY;
    }

    for (const auto &collector : energyCollectors) {
      if (collector->hitObject(ray, kSkipFrequency, hitData)) {
        return HitResult::HIT;
      }
    }

    return HitResult::NO_HIT;
  }

  void TearDown() override {
    if (energyCollectors.empty()) {
      return;
    }
    energyCollectors.clear();
  }

  void printCollectors() const {
    for (const auto &collector : energyCollectors) {
      std::cout << "Origin: " << collector->getOrigin() << "\n"
                << "Radius: " << collector->getRadius() << "\n";
    }
  }
};
// #define DEBUG
#if defined(DEBUG)

TEST_F(EnergyCollectorTest, Debug37) {
  std::cout << "DEBUG 5" << std::endl;
  const int numCollector = 5;
  energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  printCollectors();
  ASSERT_TRUE(false);
}

TEST_F(EnergyCollectorTest, Debug36) {
  std::cout << "DEBUG 4" << std::endl;
  const int numCollector = 4;
  energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  printCollectors();
  ASSERT_TRUE(false);
}
#else
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

TEST_F(EnergyCollectorTest, NotEvenNumOfEnergyCollectorCheck) {
  // This value will be used in actual simulation
  const int numCollector = 37;
  energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  // Because fake model height and size are equal to 0,
  // collectors origin distance to Vec3(0, 0, 0) is always equal to 4.
  const float collectorPositionRadius = 4;
  const float collectorAngle = 2 * kPi / (numCollector - 1);
  const float refCollectorRadius =
      collectorPositionRadius * std::sqrt(2 - 2 * std::cos(collectorAngle));

  ASSERT_EQ(energyCollectors.size(), numCollector);

  const Vec3 kVecZero(0, 0, 0);
  const Vec3 kVecUp(0, 0, 1);
  const Vec3 kVecX(1, 0, 0);
  const Vec3 kVecY(0, 1, 0);

  Ray straightUp(kVecZero, kVecUp);
  RayHitData hitData;
  ASSERT_EQ(performHitCollector(straightUp, kSkipFrequency, &hitData),
            HitResult::HIT);

  Vec3 collisionPointStraightUp =
      Vec3(0, 0, collectorPositionRadius - refCollectorRadius);
  ASSERT_EQ(hitData.collisionPoint(), collisionPointStraightUp);

  Ray straightDown(kVecZero, -kVecUp);
  ASSERT_EQ(performHitCollector(straightDown, kSkipFrequency, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();

  Ray alongX(kVecZero, kVecX);
  ASSERT_EQ(performHitCollector(alongX, kSkipFrequency, &hitData),
            HitResult::HIT);

  Ray alongY(kVecZero, kVecY);
  ASSERT_EQ(performHitCollector(alongY, kSkipFrequency, &hitData),
            HitResult::HIT);

  Ray atSixty(kVecZero, Vec3(std::cos(deg2rad(60)), 0, std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixty, kSkipFrequency, &hitData),
            HitResult::HIT);

  Ray atSixtyXY(kVecZero, Vec3(std::cos(deg2rad(60)), std::cos(deg2rad(60)),
                               std::sin(deg2rad(60))));
  ASSERT_EQ(performHitCollector(atSixtyXY, kSkipFrequency, &hitData),
            HitResult::NO_HIT)
      << "Collision Point: " << hitData.collisionPoint();
}

TEST_F(EnergyCollectorTest, TestPositionsThatWereFixed) {
  const int numCollector = 37;
  const float collectorPositionRadius = 4;

  energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  ASSERT_EQ(energyCollectors.size(), numCollector);

  // this is how previous implementation was caclualating radius of energy
  // collector
  float invalidEnergyCollectorRadius =
      2 * kPi * collectorPositionRadius / static_cast<float>(numCollector);

  RayHitData hitData;
  Ray previousNotHit1(kVecZero,
                      Vec3(0, 1.01 * invalidEnergyCollectorRadius, 1));
  ASSERT_EQ(performHitCollector(previousNotHit1, kSkipFrequency, &hitData),
            HitResult::HIT);

  Ray previousNotHit2(kVecZero,
                      Vec3(0, -1.01 * invalidEnergyCollectorRadius, 1));

  ASSERT_EQ(performHitCollector(previousNotHit2, kSkipFrequency, &hitData),
            HitResult::HIT);
}

#endif