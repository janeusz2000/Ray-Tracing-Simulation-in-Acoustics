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
    bool loadModel(std::string_view objPath);

    std::vector<objects::EnergyCollector *> getEnergyCollectors();

    std::vector<objects::TriangleObj *> getModelTriangles()
    {
        std::vector<objects::TriangleObj *> v;
        v.reserve(model_triangles_.size());
        for (const auto &t : model_triangles_)
        {
            v.push_back(t.get());
        }
        return v;
    }

    std::vector<objects::TriangleObj *> getReferencePlate();
    const objects::SphereWall &getSphereWall();

private:
    // Disallow copy and assign.
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;
    std::vector<std::unique_ptr<objects::TriangleObj>> model_triangles_;
};

#endif
