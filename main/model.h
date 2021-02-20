#ifndef MODEL_H
#define MODEL_H

#include "obj/objects.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// Holds all Triangle Objects that together represent model
// and holds all shape properties.
class ModelInterface {
public:
  virtual ~ModelInterface(){};
  virtual const std::vector<objects::TriangleObj> &triangles() const = 0;
  virtual float height() const = 0;
  virtual float sideSize() const = 0;
  // returns true if there is no object assigned to a model.
  virtual bool empty() const = 0;

  friend std::ostream &operator<<(std::ostream &os,
                                  const ModelInterface &model);

private:
  // Representation of the model for errors reading.
  virtual void printItself(std::ostream &os) const noexcept = 0;
};

class Model : public ModelInterface {
public:
  // Creates model object from given path to .obj file.
  static std::unique_ptr<Model> NewLoadFromObjectFile(std::string_view path);
  // Creates Model object that represent perfectly flat square on XY surface at
  // Z = 0, positioned at the middle of the simulation.
  // This model is made out of two equal-arm / rectangular Triangle Objects,
  // where |sideSize| represents sides length at a right angle.
  static std::unique_ptr<Model> NewReferenceModel(float sideSize);

  Model(const std::vector<objects::TriangleObj> &triangles);
  const std::vector<objects::TriangleObj> &triangles() const;

  bool empty() const override;
  void setHeight(const float height) { height_ = height_; }
  float height() const { return height_; }

  void setSideSize(const float sideSize) { sideSize_ = sideSize; }
  float sideSize() const { return sideSize_; }

private:
  // Iterates over every triangle in the model and find minimum x, y
  // dimension that cover whole model.
  float getMaxSide(const std::vector<core::Vec3> &points) const;
  // Iterates over every triangle and find minimum height
  // that covers whole model.
  float getMaxHeight(const std::vector<core::Vec3> &points) const;

  void printItself(std::ostream &os) const noexcept override;

  std::vector<objects::TriangleObj> triangles_;
  float height_, sideSize_;
};

#endif