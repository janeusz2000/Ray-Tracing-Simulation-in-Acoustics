#include "main/simulator.h"

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const AbstractModel &model, int numCollectors) {

  // We need to check if model is empty, because we can't construct energy
  // collectors around the model that doesn't exist.

  if (model.empty()) {
    throw std::invalid_argument("Empty model");
  }

  // Obviously num of collectors less then 1 cannot be allowed
  if (numCollectors < 1) {
    std::stringstream ss;
    ss << "Num collectors: " << numCollectors << " is less them 1";
    throw std::invalid_argument(ss.str());
  }

  // There are two possible configurations of EnergyCollectors:

  // a) when numCollectors is not even, we have one collector at Vec3(0, 0,
  // height), where height is equal to 4 * max(modelHeight, modelSide) and rest
  // is equally distributed on 4 sides of the model, in a way, tah distance
  // between Vec3(0, 0, 0) and each collector remains equal to height. Thats why
  // we need to make sure that for this case numCollectors - 1 % 4 == 0.

  // b) second configuration is similar to the first one, but there is no
  // center collector on Vec3(0, 0, height). So to make sure that this
  // configuration is possible, we need to check if numCollectors % 4 == 0.

  // This leaves two possible cases:
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

  // Minimum distance from Vec3(0, 0, 0) that each collector must meet.
  // Value of the minDistance can't be less then 4 [m]
  float minDistance =
      4 * std::max<float>(
              {model.height(), model.sideSize(),
               /*makes sure that minDistance is at least equal to 4 */ 1});

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

    float step = 2 * constants::kPi / numToGo;
    float groundCoordinate = minDistance * std::cos(iteration * step);
    float zCoordinate = minDistance * std::sin(iteration * step);

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