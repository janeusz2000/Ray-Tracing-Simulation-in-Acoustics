#include "core/classUtlilities.h"

void Printable::printItself(std::ostream &os) const noexcept {
  os << "NOT IMPLEMENTED!";
}

void RandomEngine::printItself(std::ostream &os) const noexcept {
  os << "RANDOM ENGINE\n"
     << "\trendering random numbers from -1 to 1";
}

RandomEngine::RandomEngine() { std::srand((unsigned int)std::time(nullptr)); }

float RandomEngine::getRandomFloat() const {
  return 2 * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) -
         1;
}
int RandomEngine::getRandomIntInRange(int min, int max) const {
  if (max < min) {
    std::stringstream ss;
    ss << "Error in RandomEngine::getRandomIntInRange()\n"
       << "Max: " << max << " cannot be bigger then min: " << min << '\n';
    throw std::invalid_argument(ss.str());
  }
  return static_cast<int>(std::rand() % (max - min + 1)) + min;
}