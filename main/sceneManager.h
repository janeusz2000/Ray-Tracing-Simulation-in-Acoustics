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
class SceneManager
{
public:
    SceneManager() : numCollectors_(constants::kPopulation), 
                    collectorRadius_(constants::kDefaultEnergyCollectorRadius),
                    simulationRadius_(constants::kDefaultSimulationRadius) {createCollectors();};
    SceneManager(size_t collectors, double simulationRadius);
    bool loadModel(std::string_view objPath);

    std::vector<objects::EnergyCollector *> getEnergyCollectors();
    std::vector<objects::TriangleObj *> getModelTriangles();
    std::vector<objects::TriangleObj *> getReferencePlate();
    const objects::SphereWall &getSphereWall();
    const generators::PointSource &getPointSource();

private:
    // Disallow copy and assign.
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    // Assigns energyCollector objects to the right positions. 
    // EnergyCollector positions creates two half-circles with the same 
    // origin at Vec3(0, 0, 0) and radius: simulationRadius / 2
    // that are crossing each other at Vec3(0, 0, SimulationRadius / 2). 
    void createCollectors();    

    std::vector<std::unique_ptr<objects::TriangleObj>> modelTriangles_;
    std::vector<std::unique_ptr<objects::EnergyCollector>> energyCollectors_;
    size_t numCollectors_;
    double collectorRadius_, simulationRadius_;
    
};

#endif
