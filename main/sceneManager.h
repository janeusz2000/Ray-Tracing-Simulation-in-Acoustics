#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "core/classUtlilities.h"
#include "core/vec3.h"
#include "main/rayTracer.h"
#include "main/simulator.h"
#include "main/trackers.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <exception>
#include <memory>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

// Store basic properties of the simulation.
// |frequencies| is vector of frequencies that simulation will be performed on.
// |sourcePower| determine how much energy is given to each
// produced ray from rayFactory.
// |numOfCollector| represents how many collectors will be used in the
// simulation.
// |numOfRaysSquared| determine how many rays will be used in the simulation.
// Note: final number of used rays in simulation will be: |numOfRaysSquared|^2.
// |maxTracking| how many reflection will simulation track per ray at maximum.
// REQUIREMENTS: |frequencies| cannot be empty, |sourcePower| must
// be positive value, |numOfCollectors| must be greater then 4 and
// |numCollectors| or  |numOfCollectors| - 1 must be divisable by 4,
// |numOfRaysSquared| greater then 0, |maxTracking| must be greater then 1
struct BasicSimulationProperties : public Printable {
  explicit BasicSimulationProperties(const std::vector<float> &frequencies,
                                     float sourcePower, int numOfCollectors,
                                     int numOfRaysSquared, int maxTracking);
  std::vector<float> frequencies;
  float sourcePower;
  int numOfCollectors;
  int numOfRaysSquared;
  int maxTracking;

  void printItself(std::ostream &os) const noexcept override;
};

// Stores all needed informations required to perform simulation.
// |energyCollectionRules| determine how energy is collected by each
// EnergyCollector.
// |basicSimulationProperies| determine how objects for simulation are built and
// on which frequencies simulation will be performed.
struct SimulationProperties : public Printable {
  explicit SimulationProperties(
      collectionRules::CollectEnergyInterface *energyCollectionRules,
      const BasicSimulationProperties &basicSimulationProperties);

  collectionRules::CollectEnergyInterface *energyCollectionRules() const {
    return energyCollectionRules_;
  }

  BasicSimulationProperties basicSimulationProperties() const {
    return basicSimulationProperties_;
  }

  void printItself(std::ostream &os) const noexcept override;

private:
  collectionRules::CollectEnergyInterface *energyCollectionRules_;
  BasicSimulationProperties basicSimulationProperties_;
};

// This class is creating all necessary objects for simulation.
class SceneManager : public Printable {
public:
  // Represents Energy Collected |first| at certain time |second|
  using EnergyPerTime = std::unordered_map<float, float>;
  // Represent collected energy value from each collector
  // at Collectors at the same index.
  using Energies = std::vector<EnergyPerTime>;
  // Represent collected energy from collectors at the given float that
  // represent frequency of the simulation.
  using EnergiesPerFrequency = std::unordered_map<float, Energies>;

  // TODO: pack this constructro into porperties class to simplify it
  explicit SceneManager(Model *model,
                        const SimulationProperties &simulationProperties,
                        trackers::PositionTrackerInterface *positionTracker,
                        trackers::CollectorsTrackerInterface *collectorsTracker,
                        ReflectionEngineInterface *reflectionEngineInterface);

  // Runs simulation and retruns map of collectors with acquired energy per
  // frequency
  std::unordered_map<float, Collectors>
  run(const CollectorBuilderInterface *collectorBuilder);

  std::unordered_map<float, Collectors>
  newRun(const CollectorBuilderInterface *collectorBuilder);

  void printItself(std::ostream &os) const noexcept override;

private:
  Model *model_;
  SimulationProperties simulationProperties_;
  RayTracer raytracer_;
  trackers::PositionTrackerInterface *positionTracker_;
  trackers::CollectorsTrackerInterface *collectorsTracker_;
  ReflectionEngineInterface *reflectionEngine_;
  std::unique_ptr<generators::RandomRayOffseter> offseter_;
};

#endif
