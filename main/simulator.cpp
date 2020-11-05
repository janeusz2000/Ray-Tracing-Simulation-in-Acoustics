#include "main/simulator.h"

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const Model &model, int numCollectors) {

  // There are two possible configurations of EnergyCollectors:
  // a) when numCollectors is not even, we have one collector at Vec3(0, 0,
  // height), and rest is equally distributed on 4 sides of the model. Thats why
  // we need to make sure that for this case numCollectors - 1 % 4 == 0, to
  // distribute collectors equally on each side. b) when numCollectors is even,
  // the is no center collector, so in order to make sure, that collectors are
  // distributed equally on each side, so thats why need to check if
  // numCollectors % 4 == 0. That leaves two possible cases:
  // 1. numCollectors % 4 == 0
  // 2. numCollectors - 1 % 4 == 0

  if (!(numCollectors % 4 == 0 || (numCollectors - 1) % 4 == 0)) {
    std::stringstream ss;
    ss << "invalid numCollectors value \n"
       << "value %4 or value -1 % 4 should be equal to 0 \n"
       << "numCollectors % 4 = " << numCollectors % 4 << "\n"
       << "numCollectors -1 % 4 = " << numCollectors - 1 % 4;
    throw std::invalid_argument(ss.str());
  }

  // minimum distance from Vec3(0, 0, 0) that each collector must meet
  float minDistance = 4 * std::max(model.height(), model.sideSize());

  // Radius of EnergyCollectors that guarantees equally coverage of 3D space
  // between each collector
  float energyCollectorRadius = constants::kPi * minDistance / numCollectors;

  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors;
  int numToGo;

  // If num Collector is even we do 1st case:
  if (numCollectors % 2 == 0) {
    numToGo = numCollectors;
  }
  // if num Collector is not even we do 2nd case:
  else {
    energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
        core::Vec3(0, 0, minDistance), energyCollectorRadius));
    numToGo = numCollectors - 1;
  }

  // equally distribution of remaining number of collectors on each side of the
  // model
  std::vector<core::Vec3> origins;
  for (int iteration = 0; iteration * 4 < numToGo; ++iteration) {

    float groundCoordinate =
        minDistance * std::cos(2 * constants::kPi * iteration / numToGo);
    float zCoordinate =
        minDistance * std::sin(2 * constants::kPi * iteration / numToGo);

    origins = {core::Vec3(groundCoordinate, 0, zCoordinate),
               core::Vec3(0, -groundCoordinate, zCoordinate),
               core::Vec3(-groundCoordinate, 0, zCoordinate),
               core::Vec3(0, groundCoordinate, zCoordinate)};

    for (const core::Vec3 origin : origins) {
      energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(
          origin, energyCollectorRadius));
    }
  }
  return energyCollectors;
}