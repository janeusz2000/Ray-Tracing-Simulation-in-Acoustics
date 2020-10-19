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
    std::vector<std::unique_ptr<objects::TriangleObj>> _modelTriangles;
    std::vector<std::unique_ptr<objects::EnergyCollector>> _energyCollectors;

    //  METHODS
    void createCollectors();
};

#endif
