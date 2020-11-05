#ifndef Model_H
#define Model_H

#include "obj/objects.h"

#include <memory>
#include <vector>

class Model {
public:
  // Model that simulation will be performend on
  std::vector<objects::TriangleObj *> triangles() const;

  static std::unique_ptr<Model> NewLoadFromObjectFile(std::string_view path);
  static std::unique_ptr<Model> NewReferenceModel(float sideSize);

  float height() const { return height_; }
  float sideSize() const { return sideSize_; }
  bool empty() const { return triangles_.empty(); }

private:
  std::vector<std::unique_ptr<objects::TriangleObj>> triangles_;
  float height_, sideSize_;
};

#endif