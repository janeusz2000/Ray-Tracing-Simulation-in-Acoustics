#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
const float kAccuracy = 0.000005;
const float kSoundSpeed = 343.216; // This is measured in 20'C at 1000 hPa
const float kPi = std::acos(-1);
const float kHitAccuracy = 0.0001;

} // namespace constants

#endif