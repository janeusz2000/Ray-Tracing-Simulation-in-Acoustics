#include "main/simulator.h"
#include "gtest/gtest.h"

const float kSkipFreq = 1000;
const size_t numOfRaysPerRow = 1000;
std::string_view skipPath = "";

class ReferenceTest : public ::testing::Test {
protected:
  Simulator sim;

  void SetUp() override {
    sim = Simulator(kSkipFreq, numOfRaysPerRow, constants::kPopulation,
                    constants::kDefaultSimulationRadius, skipPath);
  }
};