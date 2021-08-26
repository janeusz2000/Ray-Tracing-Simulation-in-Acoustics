#ifndef CLASS_UTILITIES_H
#define CLASS_UTILITIES_H

#include <boost/core/noncopyable.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

class Printable {
public:
  virtual ~Printable(){};
  virtual void printItself(std::ostream &os) const noexcept;
};

inline std::ostream &operator<<(std::ostream &os, const Printable &printable) {
  printable.printItself(os);
  return os;
}

struct RandomEngine : public Printable, private boost::noncopyable {
  explicit RandomEngine();
  float getRandomFloat() const;
  void printItself(std::ostream &os) const noexcept override;
};

#endif