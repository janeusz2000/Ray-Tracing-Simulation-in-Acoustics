#include "main/sceneManager.h"

using Tri = objects::TriangleObj;

void IamSimulator()
{
    SceneManager manager;
    CHECK(manager.loadModel("123231")) << "Cannot load model";

    std::vector<Tri> triangles = manager.getModelTriangles();
    generators::PointSource source;

    for (int i = 0; i < 1000000; i++)
    {
        auto ray = source.generateRay(1, 2);

        for (const Tri &triangle : triangles)
        {
            core::RayHitData hitData;
            if (triangle.hitObject(ray, 123123, &hitData))
            {
                ///
            }
        }
    }
}