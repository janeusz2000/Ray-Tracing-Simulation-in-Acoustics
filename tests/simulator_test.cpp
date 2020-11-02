#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

using constants::kSkipFreq;
using constants::kSkipPath;

class ReferencePlateTest : public ::testing::Test {
protected:
  Simulator sim;
};

TEST_F(ReferencePlateTest, babyTest) {
  sim.run();
  sim.printEnergy();
  ASSERT_TRUE(false);
}
