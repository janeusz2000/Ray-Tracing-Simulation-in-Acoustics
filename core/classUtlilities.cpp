#include "core/classUtlilities.h"

void Printable::printItself(std::ostream &os) const noexcept {
  os << "NOT IMPLEMENTED!";
}

void RandomEngine::printItself(std::ostream &os) const noexcept {
  os << "RANDOM ENGINE\n"
     << "\trendering random numbers from -1 to 1";
}

RandomEngine::RandomEngine() { std::srand((unsigned int)std::time(nullptr)); }

float RandomEngine::getRandomFloat() const { return std::rand(); }