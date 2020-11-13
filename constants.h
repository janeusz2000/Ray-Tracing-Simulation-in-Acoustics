#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
const double kAccuracy = 0.000005;
const double kSoundSpeed = 343.216; // This is measured in 20'C at 1000 hPa
const double kPi = std::acos(-1);
const double kHitAccuracy = 0.0001;

} // namespace constants

#endif