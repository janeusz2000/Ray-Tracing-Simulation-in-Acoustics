#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/constants.h"
#include "main/resultsCalculation.h"

#include <cmath>
#include <iostream>
#include <vector>

// Checks if every element in the array is near equal to expected array.
// if given arrays have different sizes, test will fail and show at up to which
// index elements are equal.
#define ASSERT_ELEMENTS_NEAR(TESTED_ARRAY, EXPECTED_ARRAY, TOLERANCE)          \
  __typeof__((TESTED_ARRAY).cbegin()) testIter = (TESTED_ARRAY).cbegin();      \
  __typeof__((EXPECTED_ARRAY).cbegin()) expectedIter =                         \
      (EXPECTED_ARRAY).cbegin();                                               \
  size_t currentIndex = 0;                                                     \
  while (testIter != (TESTED_ARRAY).cend() &&                                  \
         expectedIter != (EXPECTED_ARRAY).cend()) {                            \
    ASSERT_NEAR(*testIter, *expectedIter, TOLERANCE)                           \
        << "Elements are not equal at index: " << currentIndex;                \
    ++currentIndex;                                                            \
    ++testIter;                                                                \
    ++expectedIter;                                                            \
  }                                                                            \
  if (!(testIter == (TESTED_ARRAY).cend() &&                                   \
        expectedIter == (EXPECTED_ARRAY).cend())) {                            \
    ADD_FAILURE() << "Containers have different sizes\n"                       \
                  << "Elements are equal up to: " << currentIndex << " index"; \
  }

TEST(AssertElementsNear, MacroTest) {
  std::vector<float> vec1 = {1, 2.00001, 3, 4, 5, 6, 7, 8};
  std::vector<float> vec2 = {1, 2, 3, 4, 5, 6, 7, 8};
  ASSERT_ELEMENTS_NEAR(vec1, vec2, 1e-3f);
}

using Collectors = std::vector<objects::EnergyCollector *>;

using ::testing::Each;
using ::testing::ElementsAreArray;
using ::testing::IsEmpty;
using ::testing::Not;

const float kSkipDuration = 10;

TEST(WaveObject, EnergyManagementTest) {
  WaveObject basicWave;
  float t0 = 2.0f;
  basicWave.addEnergyAtTime(t0, 0);
  ASSERT_THAT(basicWave.getData(), Not(IsEmpty()));
  ASSERT_THAT(basicWave.getData(), Each(0.0f));
  ASSERT_EQ(basicWave.length(), basicWave.getSampleRate() * t0);

  float t1 = 1.0f;
  float energy = 100;
  basicWave.addEnergyAtTime(t1, energy);
  ASSERT_THAT(basicWave.getData(), Not(Each(0.0f)));
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), energy);
  ASSERT_EQ(basicWave.length(), basicWave.getSampleRate() * t0);

  basicWave.addEnergyAtTime(t1, energy);
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), 2 * energy);

  // Adding energy at the time which exceed previous time values, should
  // not have influence on data that was registered at smaller time values.
  float t2 = 40;
  basicWave.addEnergyAtTime(t2, 100);
  ASSERT_EQ(basicWave.length(), basicWave.getSampleRate() * t2);
  ASSERT_FLOAT_EQ(basicWave.getEnergyAtTime(t1), 2 * energy)
      << "resizing wave has impact on previous values!\n";
}

class WaveObjectPressureTest : public ::testing::Test {
protected:
  std::vector<float> getZeroValues(float duration, int sampleRate) const {
    return std::vector<float>(static_cast<int>(duration * sampleRate), 0);
  }
  std::vector<float> getOneValues(float duration, int sampleRate) const {
    return std::vector<float>(static_cast<int>(duration * sampleRate), 1.0f);
  }

  void assignValuesToWave(WaveObject &wave, const std::vector<float> &values) {

    for (size_t index = 0; index < values.size(); ++index) {
      float currentTime = static_cast<float>(index) / wave.getSampleRate();
      wave.addEnergyAtTime(currentTime, values[index]);
    }
  }
};

TEST_F(WaveObjectPressureTest, FixtureClassUtilsTest) {

  WaveObject defaultWave;
  ASSERT_THAT(getOneValues(kSkipDuration, defaultWave.getSampleRate()),
              Not(IsEmpty()));
  ASSERT_THAT(getZeroValues(kSkipDuration, defaultWave.getSampleRate()),
              Not(IsEmpty()));

  ASSERT_THAT(getOneValues(kSkipDuration, defaultWave.getSampleRate()),
              Each(1.0f));
  ASSERT_THAT(getZeroValues(kSkipDuration, defaultWave.getSampleRate()),
              Each(0.0f));

  std::vector<float> customVec = {1, 2, 3, std::sqrt(3), -132.09, 0};
  WaveObject wave;
  assignValuesToWave(wave, customVec);
  ASSERT_ELEMENTS_NEAR(wave.getData(), customVec, 1e-4);
}

TEST_F(WaveObjectPressureTest, GetTotalPressureTest) {

  float duration = std::sqrt(5);

  WaveObject zerosWave;
  assignValuesToWave(zerosWave,
                     getZeroValues(duration, zerosWave.getSampleRate()));
  float referencePressureFromZeros = 0;
  ASSERT_FLOAT_EQ(zerosWave.getTotalPressure(), referencePressureFromZeros);

  WaveObject onesWave;
  assignValuesToWave(onesWave,
                     getOneValues(duration, onesWave.getSampleRate()));
  std::vector<float> referenceOnes(
      std::floor(duration * onesWave.getSampleRate()), 1);
  ASSERT_ELEMENTS_NEAR(onesWave.getData(), referenceOnes, 1);
  float referencePressureFromOnes = 1 * duration;
  // ASSERT_FLOAT_EQ(onesWave.getTotalPressure(), referencePressureFromOnes);

  // float scaleFactor = 0.5f;
  // std::vector<float> customValues = getZeroValues(duration);
  // for (size_t index = 0;
  //      index < static_cast<size_t>(customValues.size() * scaleFactor);
  //      ++index) {
  //   customValues[index] = 1;
  // }
  // WaveObject wave;
  // assignValuesToWave(wave, customValues);
  // float customDuration =
  //     static_cast<float>(wave.length()) / wave.getSampleRate();
  // float referencePressureFromCustom = 1 * customDuration * scaleFactor;
  // ASSERT_FLOAT_EQ(wave.getTotalPressure(), referencePressureFromCustom);
}
