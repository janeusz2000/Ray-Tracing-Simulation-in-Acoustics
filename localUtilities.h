#ifndef LOCALUTILITIES_H
#define LOCALUTILITIES_H

#include "constants.h"

namespace localUtilities {
float deg2rad(float angle) { return 2 * constants::kPi * angle / 360; }
} // namespace localUtilities

#endif