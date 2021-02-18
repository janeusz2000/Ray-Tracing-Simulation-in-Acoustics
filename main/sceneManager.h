#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "core/vec3.h"
#include "main/positionTracker.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <future>
#include <memory>
#include <queue>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

// Holds all needed information to perform simulation.
struct SimulationProperties {
  explicit SimulationProperties(
      std::string_view dataPath, const std::vector<float> &frequencies /*[Hz]*/,
      collectionRules::CollectEnergyInterface *energyCollectionRules,
      float sourcePower = 500 /*[W]*/, int numOfCollectors = 37,
      int numOfRaySquared = 1000)
      : dataPath_(dataPath.data()), frequencies_(frequencies),
        energyCollectionRules_(energyCollectionRules),
        sourcePower_(sourcePower), numOfCollectors_(numOfCollectors),
        numOfRaySquared_(numOfRaySquared) {}

  const std::vector<float> &frequencies() const { return frequencies_; }
  collectionRules::CollectEnergyInterface *energyCollectionRules() {
    return energyCollectionRules_;
  }
  const float &sourcePower() const { return sourcePower_; }
  const int &numOfCollectors() const { return numOfCollectors_; }
  const int &numOfRaySquared() const { return numOfRaySquared_; }
  const char *dataPath() const { return dataPath_.data(); }

  collectionRules::CollectEnergyInterface *collectionRules() {
    return energyCollectionRules_;
  }

private:
  std::string_view dataPath_;
  std::vector<float> frequencies_;
  collectionRules::CollectEnergyInterface *energyCollectionRules_;
  float sourcePower_;
  int numOfCollectors_;
  int numOfRaySquared_;
};

// This class is creating all necessary objects for simulation.
class SceneManager {
public:
  explicit SceneManager(Model *model,
                        const SimulationProperties &simulationProperties);

  void run();

private:
  Model *model_;
  SimulationProperties simulationProperties_;

  RayTracer raytracer_;
  trackers::PositionTracker tracker_;
  generators::PointSpeakerRayFactory pointSpeaker_;

  std::unique_ptr<generators::RandomRayOffseter> offseter_;
  std::unique_ptr<Model> referenceModel_;
};

#endif
