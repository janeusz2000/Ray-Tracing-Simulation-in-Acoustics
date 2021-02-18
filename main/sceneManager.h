#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "core/vec3.h"
#include "main/positionTracker.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <memory>
#include <string_view>
#include <vector>

// This class is creating all necessary objects for simulation.
class SceneManager {
public:
  SceneManager(Model *model, const std::vector<float> &frequencies /*[Hz]*/,
               std::string_view dataPath,
               collectionRules::CollectEnergyInterface *energyCollectionRules,
               float sourcePower = 500 /*[W]*/, int numOfCollectors = 37,
               int numOfRayRaysSquared = 1000);

  void run();

private:
  Model *model_;
  std::vector<float> frequencies_;
  collectionRules::CollectEnergyInterface *energyCollectionRules_;
  int numOfCollectors_;

  RayTracer raytracer_;
  trackers::PositionTracker tracker_;
  generators::PointSpeakerRayFactory pointSpeaker_;

  std::unique_ptr<generators::RandomRayOffseter> offseter_;
  std::unique_ptr<Model> referenceModel_;
};

#endif
