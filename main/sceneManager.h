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

// TODO: change underscore to the end

// This class is creating all necessary objects for simulation.
class SceneManager
{
public:
    SceneManager();
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

    void createCollectors();    // assigns energyCollector objects to the right positions. 
                                // EnergyCollector positions creates two half-circles with teh same 
                                // origin at Vec3(0, 0, 0) that are crossing each other at 
                                // Vec3(0, 0, kSimulationRadius / 2)

    std::vector<std::unique_ptr<objects::TriangleObj>> _modelTriangles;
    std::vector<std::unique_ptr<objects::EnergyCollector>> _energyCollectors;
    
};

#endif
