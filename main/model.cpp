#include "main/model.h"

const std::vector<objects::TriangleObj> &Model::triangles() const {
  return triangles_;
}

Model::Model(const std::vector<objects::TriangleObj> &triangles)
    : triangles_(triangles) {

  // finding maximum side length and maximum height of the model.
  float maxSideSize = 0, maxHeight = 0;
  for (const auto &triangle : triangles_) {

    maxSideSize = std::max(maxSideSize, getMaxSide(triangle.getPoints()));
    maxHeight = std::max(maxHeight, getMaxHeight(triangle.getPoints()));
  }

  setSideSize(maxSideSize);
  setHeight(maxHeight);
}

std::unique_ptr<Model> Model::NewReferenceModel(float size) {

  std::vector<core::Vec3> clockWiseOrigins = {
      core::Vec3(-size / 2, size / 2, 0), core::Vec3(size / 2, size / 2, 0),
      core::Vec3(size / 2, -size / 2, 0), core::Vec3(-size / 2, -size / 2, 0)};

  std::vector<objects::TriangleObj> objects = {
      objects::TriangleObj(clockWiseOrigins[1], clockWiseOrigins[2],
                           clockWiseOrigins[3]),
      objects::TriangleObj(clockWiseOrigins[3], clockWiseOrigins[0],
                           clockWiseOrigins[1])};

  return std::make_unique<Model>(objects);
}

bool Model::empty() const {
  return std::max<float>(height(), sideSize()) <= constants::kAccuracy;
}

float Model::getMaxSide(const std::vector<core::Vec3> &points) const {
  float maxSide = 0;
  for (const core::Vec3 &point : points) {
    maxSide = std::max(std::abs(point.x()), std::abs(point.y()));
  }
  return maxSide;
}

float Model::getMaxHeight(const std::vector<core::Vec3> &points) const {
  float maxHeight = 0;
  for (const core::Vec3 &point : points) {
    maxHeight = std::max(maxHeight, point.z());
  }
  return maxHeight;
}