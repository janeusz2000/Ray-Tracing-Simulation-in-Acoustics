#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <fstream>
#include <iostream>
#include <string>

using core::Vec3;
using objects::TriangleObj;

TEST(learningTest, readingObjFile) {
  std::vector<Vec3> points;
  std::vector<TriangleObj> triangles;

  std::string path = "/tmp/simpleCube.obj";
  std::ifstream objFile;
  objFile.open(path);

  ASSERT_TRUE(objFile.good());

  for (std::string line; std::getline(objFile, line);) {
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> stringWords(begin, end);

    if (stringWords[0] == "v") {

      float x = std::stof(stringWords[1]);
      float y = std::stof(stringWords[2]);
      float z = std::stof(stringWords[3]);
      points.push_back(Vec3(x, y, z));

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
      Vec3 point1 = points[facePoints[0]];
      Vec3 point2 = points[facePoints[1]];
      Vec3 point3 = points[facePoints[2]];

      triangles.push_back(TriangleObj(point1, point2, point3));
    }
  }
  for (const auto element : triangles) {
    std::cout << element << std::endl;
  }
}