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
      : SceneManager(constants::kDefaultCollectorPopulation,
                     constants::kDefaultSimulationRadius){};
  SceneManager(int collectors, float simulationRadius);

  [[nodiscard]] bool loadModel(std::string_view objPath);

  std::vector<objects::EnergyCollector *> getEnergyCollectors() const;
  std::vector<objects::TriangleObj *> getModelTriangles() const;
  std::vector<objects::TriangleObj *> getReferencePlate() const;
  objects::SphereWall &getSphereWall() { return sphereWall_; }
  const generators::PointSource &getPointSource() { return source_; }

  float collectorRadius() const { return collectorRadius_; };
  // TODO: this should be based od load model
  float getSmapleSize() const { return 1.0f; };
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

  // Assigns square made with two triangleObjs to the referenePlate_.
  // square has origin at (0, 0, 0) and side length is equal to
  // simulationRadius/2.
  void createReferencePlate();

  generators::PointSource source_;
  objects::SphereWall sphereWall_;

  std::vector<std::unique_ptr<objects::TriangleObj>> modelTriangles_;
  std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors_;
  std::vector<std::unique_ptr<objects::TriangleObj>> referencePlate_;
  int numCollectors_;
  float collectorRadius_, simulationRadius_;
};

#endif
