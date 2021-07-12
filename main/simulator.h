#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "core/classUtlilities.h"
#include "main/rayTracer.h"
#include "main/trackers.h"
#include "nlohmann/json.hpp"
#include "obj/generators.h"
#include "obj/objects.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;
// Represents Energy Collected |first| at certain time |second|
using EnergyPerTime = std::unordered_map<float, float>;
using Energies = std::vector<EnergyPerTime>;

namespace collectionRules {

// defines how energy collectors collect energy in the simulation
struct CollectEnergyInterface : public Printable {
  virtual void collectEnergy(const Collectors &collectors,
                             core::RayHitData *hitData) = 0;
  void printItself(std::ostream &os) const noexcept override;
};

// The futher away from origin of energy collectors ray hits, the less energy it
// puts in the energy collector - left energy scales lineary. When distanece
// betwen hit position and energyCollector origin is equal or bigger then radius
// of the energyCollector, none energy is put inside the energy Collector. Phase
// impact of the wave is not considered here.
struct LinearEnergyCollection : public CollectEnergyInterface {
  void collectEnergy(const Collectors &collectors,
                     core::RayHitData *hitData) override;
  void printItself(std::ostream &os) const noexcept override;
};

// Rules of collection are exactly the same as in LinearEnergyCollection, but in
// addition energy is multiplied by cos(phase), where phase represents wave
// phase at hit position.
struct LinearEnergyCollectionWithPhaseImpact : public CollectEnergyInterface {
  void collectEnergy(const Collectors &collectors,
                     core::RayHitData *hitData) override;

  void printItself(std::ostream &os) const noexcept override;
};

// Energy collection based on the "Optimizing diffusive surface topology through
// a performance-based design approach" paper. Collected Energy is defined as
// sound intensity that occurred inside EnergyCollector. Sound Intensity is
// calculated from:
// soundIntensity = energyPerRay * distanceFactor / EnergyCollectorVolume,
// where distance factor is:
// distanceFactor = 2 * sqrt(collectorRadius^2 - distanceToOrigin^2)
struct NonLinearEnergyCollection : public CollectEnergyInterface {
  void collectEnergy(const Collectors &collectors,
                     core::RayHitData *hitData) override;
  void printItself(std::ostream &os) const noexcept override;
};
// TODO: Create Combined Rules of collection
// TODO: Add time factor to the collected energy
} // namespace collectionRules

Collectors buildCollectors(const ModelInterface *model, int numCollectors);

struct CollectorBuilderInterface : public Printable {
  virtual Collectors buildCollectors(const ModelInterface *model,
                                     int numCollectors) const = 0;
  void printItself(std::ostream &os) const noexcept override;
};

// Constructs an array of Energy Collectors around specified model.
// Energy Collectors are arranged in two half-circles, whose origin is
// centere on the model, oriented at the right angle to each other.
// Their radius is equal to 4 * max(model height, model side),
// but not less then 4.

// Collectors are arranged uniformly on the circumference's, such that bottom
// collectors are on the ground level.
// In case of odd number of collectors, one collector is placed right above the
// model. Radius of an energy collector is equal to distance between twoenergy
// collectors.

struct DoubleAxisCollectorBuilder : public CollectorBuilderInterface {

  // Throws std::invalid_argument when |numCollectors| < 4 or when
  // |numCollectors| or |numCollectors|-1 is not divisible by 4.
  Collectors buildCollectors(const ModelInterface *model,
                             int numCollectors) const override;
  void printItself(std::ostream &os) const noexcept override;
};

// Constructs an array of Energy Collectors around specified model.
// Works the same as DoubleAxisCollectorBuilder but instead it only build one
// half sphere on along X Axis.
struct XAxisCollectorBuilder : public CollectorBuilderInterface {

  Collectors buildCollectors(const ModelInterface *model,
                             int numCollectors) const override;

  void printItself(std::ostream &os) const noexcept override;
};
// Constructs an array of Energy Collectors around specified model.
// Works the same as DoubleAxisCollectorBuilder but instead it only build one
// half sphere on along Y Axis.
struct YAxisCollectorBuilder : public CollectorBuilderInterface {

  Collectors buildCollectors(const ModelInterface *model,
                             int numCollectors) const override;

  void printItself(std::ostream &os) const noexcept override;
};
// Constructs half-geodesic dome pattern of Energy Collectors around the model.
struct GeometricDomeCollectorBuilder : public CollectorBuilderInterface {
  Collectors
  buildCollectors(const ModelInterface *model,
                  /*skip this number*/ int numCollectors = 0) const override;
  void printItself(std::ostream &os) const noexcept override;

private:
  std::vector<core::Vec3> loadPoints(int size) const;
  std::vector<core::Vec3>
  findClosest5Points(const core::Vec3 &point,
                     std::vector<core::Vec3> pointList) const;
};

// Saves positions of the energyCollectors to the Json file at given path.
void exportCollectorsToJson(const Collectors &energyCollectors,
                            std::string_view path);

// Calculates maximum radius of the sphere that limits size of the
// simulation Rays cannot reach any position outside of the SphereWall
// object.
const float getSphereWallRadius(const ModelInterface &model);

// Defines how sound is reflecting from the structure
struct ReflectionEngineInterface : public Printable {
  virtual std::vector<core::Ray>
  modelReflectedSoundWave(const core::Ray &reflected) const = 0;
  void printItself(std::ostream &os) const noexcept override;
};

struct FakeReflectionEngine : public ReflectionEngineInterface {
  std::vector<core::Ray>
  modelReflectedSoundWave(const core::Ray &reflected) const override;
  void printItself(std::ostream &os) const noexcept override;
};

// Performs ray-tracing simulation on given model.
class Simulator : public Printable {
public:
  Simulator(RayTracer *tracer, ModelInterface *model,
            generators::RayFactory *source,
            generators::RandomRayOffseter *offsetter,
            trackers::PositionTrackerInterface *positionTracker,
            collectionRules::CollectEnergyInterface *energyCollectionRules,
            ReflectionEngineInterface *reflectionEngine)
      : tracer_(tracer), model_(model), source_(source), offsetter_(offsetter),
        positionTracker_(positionTracker),
        energyCollectionRules_(energyCollectionRules),
        reflectionEngine_(reflectionEngine),
        sphereWall_(getSphereWallRadius(*model)){};

  // Runs the simulation by modifying given collectors
  void run(float frequency, Collectors *collectors, const int maxTracking);
  void runRayTracing(float frequency, Collectors *collectors,
                     const int maxTracking) const;

  void printItself(std::ostream &os) const noexcept override;

private:
  void performRayTracing(Collectors *collectors, float frequency,
                         core::Ray *currentRay, int maxTracking,
                         int *currentTracking) const;
  RayTracer *tracer_;
  ModelInterface *model_;
  generators::RayFactory *source_;
  generators::RandomRayOffseter *offsetter_;

  trackers::PositionTrackerInterface *positionTracker_;
  collectionRules::CollectEnergyInterface *energyCollectionRules_;

  ReflectionEngineInterface *reflectionEngine_;
  objects::SphereWall sphereWall_;
};

#endif