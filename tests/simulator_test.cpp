#include "constants.h"
#include "main/simulator.h"
#include "gtest/gtest.h"

using constants::kSkipFreq;
using constants::kSkipPath;

class ReferenceTest : public ::testing::Test {
protected:
  Simulator sim;
};