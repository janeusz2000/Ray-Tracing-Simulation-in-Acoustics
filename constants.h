#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cmath>
#include <iostream>

namespace constants {
// Expresses accuracy of measured distance in meters in hit position
// calculation, or accuracy of time in seconds in hit time calculation
const float kAccuracy = 0.00005;
const float kAreaAccuracy = 0.0001;
// Measured in 20'C at 1000 hPa
const float kSoundSpeed = 343.216;
const float kPi = std::acos(-1);
const int kSimulationHeight = 8; // [m]
} // namespace constants

#endif