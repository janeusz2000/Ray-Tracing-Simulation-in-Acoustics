#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
// Express accurracy in meters in terms of hit position,
// or accuracy of time in hitTime calculation
const float kAccuracy = 0.00005; 
// This is measured in 20'C at 1000 hPa
const float kSoundSpeed = 343.216; 
const float kPi = std::acos(-1);
} // namespace constants

#endif