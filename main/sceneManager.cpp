#include "main/sceneManager.h"

SceneManager::SceneManager()
{
    createCollectors();
}

std::vector<objects::EnergyCollector *> SceneManager::getEnergyCollectors()
{
    std::vector<objects::EnergyCollector *> temp;
    temp.reserve(_energyCollectors.size());
    for (const auto &t : _energyCollectors)
    {
        temp.push_back(t.get());
    }
    return temp;
}

std::vector<objects::TriangleObj *> SceneManager::getModelTriangles()
{
    std::vector<objects::TriangleObj *> temp;
    temp.reserve(_modelTriangles.size());
    for (const auto &t : _modelTriangles)
    {
        temp.push_back(t.get());
    }
    return temp;
}

// PRIVATE METHODS

void SceneManager::createCollectors()
{
    _energyCollectors.reserve(constants::kCollectors);

    const double collectorRadius = static_cast<double>(constants::kSimulationRadius) / 2;
    double alphaIncrement;

    if (constants::kCollectors % 2 == 0)
    {
        alphaIncrement = 2 * constants::kPi * 180 / (static_cast<double>(constants::kCollectors) / 2 * 360);
    }
    else
    {
        alphaIncrement = 2 * constants::kPi * 180 / ((static_cast<double>(constants::kCollectors) - 1) / 2 * 360);
    }

    for (double alpha = 0; alpha <= constants::kPi; alpha += alphaIncrement)
    {
        double cur = collectorRadius * std::sin(alpha);
        double z = collectorRadius * std::cos(alpha);

        core::Vec3 xAxesOrigin(cur, 0, z);
        core::Vec3 yAxesOrigin(0, cur, z);

        if (xAxesOrigin == core::Vec3(0, 0, constants::kSimulationRadius / 2))
        {
            _energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(objects::EnergyCollector(xAxesOrigin)));
        }
        else
        {
            _energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(objects::EnergyCollector(xAxesOrigin)));
            _energyCollectors.push_back(std::make_unique<objects::EnergyCollector>(objects::EnergyCollector(yAxesOrigin)));
        }
    }
}