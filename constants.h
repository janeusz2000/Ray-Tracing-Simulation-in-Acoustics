#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
// kAccuracy express accuracy in meters in calculation of hit position, 
// or accuracy of time in seconds in hit time calculation
const float kAccuracy = 0.00005; 
// This is measured in 20'C at 1000 hPa
const float kSoundSpeed = 343.216; 
const float kPi = std::acos(-1);
} // namespace constants

#endif