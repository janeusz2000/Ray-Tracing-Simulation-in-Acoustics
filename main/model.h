#ifndef Model_H
#define Model_H

#include "obj/objects.h"

#include <memory>
#include <vector>

// Holds all Triangle Objects that represent model in simulation and adds
// some utilities for it.
class ModelInterface {
public:
  virtual ~ModelInterface(){};
  virtual std::vector<objects::TriangleObj *> triangles() const = 0;
  virtual double height() const = 0;
  virtual double sideSize() const = 0;
  // returns true if there is no object assigned to a model.
  virtual bool empty() const = 0;
};

class Model : public ModelInterface {
public:
  std::vector<objects::TriangleObj *> triangles() const;

  double height() const { return height_; }
  double sideSize() const { return sideSize_; }
  bool empty() const override {
    return std::max<double>(height(), sideSize()) <= constants::kAccuracy;
  }

  static std::unique_ptr<Model> NewLoadFromObjectFile(std::string_view path);
  static std::unique_ptr<Model> NewReferenceModel(double sideSize);

private:
  std::vector<std::unique_ptr<objects::TriangleObj>> triangles_;
  double height_, sideSize_;
};

#endif