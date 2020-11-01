#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

// This class is creating all necessary objects for simulation.
class SceneManager {
public:
  SceneManager()
      : SceneManager(constants::kPopulation,
                     constants::kDefaultSimulationRadius){};
  SceneManager(int collectors, float simulationRadius);

  bool loadModel(std::string_view objPath);

  std::vector<objects::EnergyCollector *> getEnergyCollectors() const;
  std::vector<objects::TriangleObj *> getModelTriangles() const;
  std::vector<objects::TriangleObj *> getReferencePlate() const;
  const objects::SphereWall &getSphereWall();
  const generators::PointSource &getPointSource();

  float collectorRadius() const { return collectorRadius_; };
  float simulatiorRadius() const { return simulationRadius_; };

private:
  // Disallow copy and assign.
  SceneManager(const SceneManager &) = delete;
  SceneManager &operator=(const SceneManager &) = delete;

  // Assigns energyCollector objects to the right positions.
  // EnergyCollector positions creates two half-circles with the same
  // origin at Vec3(0, 0, 0) and radius: simulationRadius / 2
  // that are crossing each other at Vec3(0, 0, SimulationRadius / 2).
  void createCollectors();

  // Assigns square made with two triangleObjs to the referenePlate_
  // Triangles have points at:
  //  - Vec3(simRadius/4, simRadius/4, 0),
  //  - Vec3(simRadius/4, -simRadius/4, 0),
  //  - Vec3(-simRadius/4, simRadius/4, 0),
  //  - Vec3(-simRadius/4, -simRadius/4, 0)
  void createReferencePlate();

  std::vector<std::unique_ptr<objects::TriangleObj>> modelTriangles_;
  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors_;
  std::vector<std::unique_ptr<objects::TriangleObj>> referencePlate_;
  int numCollectors_;
  float collectorRadius_, simulationRadius_;
};

#endif
