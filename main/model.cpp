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

Model Model::NewLoadFromObjectFile(std::string_view path) {

  // TODO: delete triangle if area is to small
  // TODO: check if path of the file is good
  // TODO: check why objects are turned
  // TODO: check when first element is v or f if data is good
  // TODO: find out why only few of the triangles are being imported.

  std::vector<core::Vec3> points;
  std::vector<objects::TriangleObj> triangles;
  std::ifstream objFile;
  objFile.open(path.data());

  for (std::string line; std::getline(objFile, line);) {
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> stringWords(begin, end);

    if (stringWords[0] == "v") {

      float x = std::stof(stringWords[1]);
      float y = std::stof(stringWords[2]);
      float z = std::stof(stringWords[3]);
      points.push_back(core::Vec3(x, y, z));

    } else if (stringWords[0] == "f") {
      std::vector<int> facePoints;
      for (size_t wordIndex = 1; wordIndex < stringWords.size(); ++wordIndex) {

        size_t found = stringWords[wordIndex].find("/");
        if (found != std::string::npos) {
          facePoints.push_back(
              std::stoi(stringWords[wordIndex].substr(0, found)) - 1);
        } else {
          facePoints.push_back(std::stoi(stringWords[wordIndex]) - 1);
        }
      }
      core::Vec3 point1 = points[facePoints[0]];
      core::Vec3 point2 = points[facePoints[1]];
      core::Vec3 point3 = points[facePoints[2]];

      triangles.push_back(objects::TriangleObj(point1, point2, point3));
    }
  }
  return Model(triangles);
}

Model Model::NewReferenceModel(float size) {

  std::vector<core::Vec3> clockWiseOrigins = {
      core::Vec3(-size / 2, size / 2, 0), core::Vec3(size / 2, size / 2, 0),
      core::Vec3(size / 2, -size / 2, 0), core::Vec3(-size / 2, -size / 2, 0)};

  std::vector<objects::TriangleObj> objects = {
      objects::TriangleObj(clockWiseOrigins[1], clockWiseOrigins[2],
                           clockWiseOrigins[3]),
      objects::TriangleObj(clockWiseOrigins[3], clockWiseOrigins[0],
                           clockWiseOrigins[1])};

  return Model(objects);
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