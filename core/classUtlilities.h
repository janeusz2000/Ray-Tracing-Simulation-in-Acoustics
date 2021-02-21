#ifndef CLASS_UTILITIES_H
#define CLASS_UTILITIES_H

#include <iostream>

class Printable {
public:
  virtual void printItself(std::ostream &os) const noexcept;
  friend std::ostream &operator<<(std::ostream &os, const Printable &printable);
};

#endif