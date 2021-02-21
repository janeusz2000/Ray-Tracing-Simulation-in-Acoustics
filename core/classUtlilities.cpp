#include "core/classUtlilities.h"

std::ostream &operator<<(std::ostream &os, const Printable &printable) {
  printable.printItself(os);
  return os;
}

void Printable::printItself(std::ostream &os) const noexcept {
  os << "NOT IMPLEMENTED!";
}
