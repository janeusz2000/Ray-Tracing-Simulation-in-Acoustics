#ifndef Model_H
#define Model_H

#include "obj/objects.h"

#include <memory>
#include <vector>

class AbstractModel {
public:
  virtual ~AbstractModel(){};
  // Model that simulation will be performend on
  virtual std::vector<objects::TriangleObj *> triangles() const = 0;
  virtual float height() const = 0;
  virtual float sideSize() const = 0;
  virtual bool empty() const {
    return std::max<float>(height(), sideSize()) <= constants::kAccuracy;
  }
};

class Model : public AbstractModel {
public:
  // Model that simulation will be performend on
  std::vector<objects::TriangleObj *> triangles() const;

  static std::unique_ptr<Model> NewLoadFromObjectFile(std::string_view path);
  static std::unique_ptr<Model> NewReferenceModel(float sideSize);

  float height() const { return height_; }
  float sideSize() const { return sideSize_; }

private:
  std::vector<std::unique_ptr<objects::TriangleObj>> triangles_;
  float height_, sideSize_;
};

#endif