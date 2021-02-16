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

std::unique_ptr<Model> Model::NewLoadFromObjectFile(std::string_view path) {
  std::vector<core::Vec3> points;
  std::vector<objects::TriangleObj> triangles;
  std::ifstream objFile;

  // Check if file exist at given path
  objFile.open(path.data());
  if (!objFile.good()) {
    std::stringstream errorStream;
    errorStream << "Invalid path of the .obj \n"
                << "Path: " << path.data();
    throw std::invalid_argument(errorStream.str());
  }

  int lineNumber = 1;
  for (std::string line; std::getline(objFile, line); ++lineNumber) {
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> stringWords(begin, end);

    if (stringWords[0] == "v") {

      // Checks if point declaration is valid
      if (stringWords.size() != 4) {
        std::stringstream errorString;
        errorString << "Invalid point declaration in object file at:\n"
                    << "line: " << lineNumber;
      }

      float x = std::stof(stringWords[1]);
      float z = std::stof(stringWords[2]);
      float y = std::stof(stringWords[3]);
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

      // Check if current line in .obj contains triangle
      if (facePoints.size() > 3) {
        std::stringstream errorStringStream;
        errorStringStream << "Objects must consit of triangles only! \n"
                          << "line: " << lineNumber << ", in: " << path.data()
                          << "\n"
                          << "Acquired points: \n[";
        for (const auto &str : facePoints) {
          errorStringStream << str << ", ";
        }
        throw std::invalid_argument(errorStringStream.str());
      }

      core::Vec3 point1 = points[facePoints[0]];
      core::Vec3 point2 = points[facePoints[1]];
      core::Vec3 point3 = points[facePoints[2]];

      try {
        objects::TriangleObj triangle(point1, point2, point3);
        triangles.push_back(objects::TriangleObj(point1, point2, point3));
      } catch (const std::exception &e) {
        std::cout << "WARNING! \n"
                  << e.what() << "\n"
                  << "It wont be included in simulation" << std::endl;
      }
    }
  }
  return std::make_unique<Model>(triangles);
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