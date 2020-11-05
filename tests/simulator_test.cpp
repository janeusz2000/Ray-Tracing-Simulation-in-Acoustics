#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

using constants::kPi;
using core::Ray;
using core::RayHitData;
using core::Vec3;

const int kSkipNumCollectors = 37;
const float kSkipFrequency = 1000;

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

TEST_F(EnergyCollectorTest, EnergyCollectorPositionCheck) {
  // This value will be used in actual simulation
  const int numCollector = 37;
  energyCollectors = buildCollectors(nonEmptyModel, numCollector);
  // Because fake model height and size are equal to 0,
  // collectors origin distance to Vec3(0, 0, 0) is always equal to 4.
  const float collectorPositionRadius = 4;
  const float refCollectorRadius = kPi * 4 / static_cast<float>(numCollector);

  ASSERT_EQ(energyCollectors.size(), numCollector);

  const Vec3 kVecZero(0, 0, 0);
  Ray straightUp(kVecZero, Vec3(0, 0, 1));
  RayHitData hitData;
  ASSERT_EQ(performHitCollector(straightUp, kSkipFrequency, &hitData),
            HitResult::HIT);

  Vec3 collisionPointStraightUp =
      Vec3(0, 0, collectorPositionRadius - refCollectorRadius);
  ASSERT_EQ(hitData.collisionPoint(), collisionPointStraightUp);
}
