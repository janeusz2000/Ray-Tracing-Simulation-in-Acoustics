#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/constants.h"
#include "main/resultsCalculation.h"

#include <cmath>
#include <vector>

using Collectors = std::vector<objects::EnergyCollector *>;
const int kDefaultSampleRate = 96e3;

TEST(WaveObject, EnergyManagement) {
  WaveObject basicWave;
  float t0 = 2.0f;
  basicWave.addEnergyAtTime(t0, 0);
  ASSERT_THAT(basicWave.getData(), ::testing::Not(::testing::IsEmpty()));
  ASSERT_THAT(basicWave.getData(), ::testing::Each(0.0f));
  ASSERT_EQ(basicWave.length(), kDefaultSampleRate * t0);

  float t1 = 1.0f;
  float energy = 100;
  basicWave.addEnergyAtTime(t1, energy);
  ASSERT_THAT(basicWave.getData(), ::testing::Not(::testing::Each(0.0f)));
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), energy);
  ASSERT_EQ(basicWave.length(), kDefaultSampleRate * t0);

  basicWave.addEnergyAtTime(t1, energy);
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), 2 * energy);

  float t2 = 4.0f;
  basicWave.addEnergyAtTime(t2, 100);
  ASSERT_EQ(basicWave.length(), kDefaultSampleRate * t2);
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), 2 * energy)
      << "resizing wave has impact on previous values!\n";
}

class WaveObjectPressureTest : public ::testing::Test {
public:
  explicit WaveObjectPressureTest(float amplitude = 1.0f, float frequency = 1e3,
                                  float duration = 1e3)
      : amplitude_(amplitude), frequency_(frequency) {

    assignValuesToWave(zeros_, getZeroValues());
    assignValuesToWave(ones_, getOneValues());
    assignValuesToWave(sin_, getSineValues());
    assignValuesToWave(square_, getSquareValues());
  };

protected:
  const float amplitude() const { return amplitude_; }
  const float frequency() const { return frequency_; }
  const float duration() const { return duration_; }

  const WaveObject &getZeros() const { return zeros_; }
  const WaveObject &getOnes() const { return ones_; }
  const WaveObject &getSquare() const { return square_; }
  const WaveObject &getSine() const { return sin_; }

private:
  std::vector<float> getAngleVec() const {
    std::vector<float> angleValues;
    size_t outputLength = static_cast<int>(duration_ * kDefaultSampleRate);
    for (size_t index = 0; index < outputLength; ++index) {
      angleValues.push_back(
          index * (2.0f * constants::kPi * frequency_ / kDefaultSampleRate));
    }
    return angleValues;
  }
  std::vector<float> getZeroValues() const {
    return std::vector<float>(static_cast<int>(duration_ * kDefaultSampleRate),
                              0);
  }
  std::vector<float> getOneValues() const {
    return std::vector<float>(static_cast<int>(duration_ * kDefaultSampleRate),
                              1.0f);
  }
  std::vector<float> getSineValues() const {
    std::vector<float> sineValues;
    for (const float angle : getAngleVec()) {
      sineValues.push_back(amplitude_ * std::sin(angle));
    }
    return sineValues;
  }
  std::vector<float> getSquareValues() const {
    std::vector<float> sin = getSineValues();
    std::vector<float> square;
    for (const float value : sin) {
      float squareValue = value >= 0 ? 1 : 0;
      square.push_back(squareValue);
    }
    return square;
  }
  void assignValuesToWave(WaveObject &wave, const std::vector<float> &values) {
    size_t outputLength = static_cast<int>(duration_ * kDefaultSampleRate);
    for (size_t index = 0; index < outputLength; ++index) {
      float currentTime = index * kDefaultSampleRate;
      wave.addEnergyAtTime(currentTime, values[index]);
    }
  }

  float amplitude_;
  float frequency_;
  float duration_;

  WaveObject zeros_;
  WaveObject ones_;
  WaveObject sin_;
  WaveObject square_;
};

TEST_F(WaveObjectPressureTest, membersTestClass) {}

TEST_F(WaveObjectPressureTest, getTotalPressure) {
  //   float referencePressureFromOnes = 1 * duration();
  //   ASSERT_FLOAT_EQ(getOnes().getTotalPressure(), referencePressureFromOnes);
  //   // TODO: calculate integral after zeros, ones, sin, square
}
