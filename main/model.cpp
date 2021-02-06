#include "main/model.h"

std::vector<objects::TriangleObj *> Model::triangles() const {
  std::vector<objects::TriangleObj *> outputTriangles;
  outputTriangles.reserve(triangles_.size());
  for (const auto &triangle : triangles_) {
    outputTriangles.push_back(triangle.get());
  }
  return outputTriangles;
}

Model::Model(
    const std::vector<std::unique_ptr<objects::TriangleObj>> &triangles)
    : triangles_(triangles) {

  // finding maximum side and maximum height that will cover whole model
  float maxSideSize = 0, maxHeight = 0;
  for (const auto &triangle : triangles_) {

    float tempMaxSideSize = std::max(std::abs(triangle->getOrigin().x()),
                                     std::abs(triangle->getOrigin().y()));
    if (maxSideSize < tempMaxSideSize) {
      maxSideSize = tempMaxSideSize;
    }

    float tempMaxHeight = std::abs(triangle->getOrigin().z());

    if (maxHeight < tempMaxHeight) {
      maxHeight = tempMaxHeight;
    }
  }

  setSideSize(maxSideSize);
  setHeight(maxHeight);
}

std::unique_ptr<Model> Model::NewReferenceModel(float size) {

  std::vector<core::Vec3> clockWiseOrigins = {
      core::Vec3(-size / 2, size / 2, 0), core::Vec3(size / 2, size / 2, 0),
      core::Vec3(size / 2, -size / 2, 0), core::Vec3(-size / 2, -size / 2, 0)};

  std::vector<std::unique_ptr<objects::TriangleObj>> objects = {
      std::make_unique<objects::TriangleObj>(
          clockWiseOrigins[1], clockWiseOrigins[2], clockWiseOrigins[3]),
      std::make_unique<objects::TriangleObj>(
          clockWiseOrigins[3], clockWiseOrigins[0], clockWiseOrigins[1])};

  return std::make_unique<Model>(objects);
}
