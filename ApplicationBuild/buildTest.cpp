#include "core/classUtlilities.h"
#include <iostream>

int main() {

  RandomEngine randomEngine;

  for (int index = 0; index < 1000; index++) {
    std::cout << randomEngine.getRandomFloat() << std::endl;
  }
  return 0;
}
