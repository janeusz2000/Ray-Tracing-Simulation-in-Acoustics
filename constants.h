#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants {
// Expresses accuracy in meters in hit position calculation,
// or accuracy of time in seconds in hit time calculation
const float kAccuracy = 0.00005;
// Measured in 20'C at 1000 hPa
const float kSoundSpeed = 343.216;
const float kPi = std::acos(-1);

} // namespace constants

#endif