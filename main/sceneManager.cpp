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

    const double collectorRadius = constants::kSimulationRadius / 2;
    double alphaIncrement;

    if (constants::kCollectors % 2 == 0)
    {
        alphaIncrement = constants::kPi * 180 / (static_cast<double>(constants::kCollectors) / 2);
    }
    else
    {
        alphaIncrement = constants::kPi * 180 / ((static_cast<double>(constants::kCollectors) - 1) / 2);
    }

    for (double alpha = 0; alpha <= 180; alpha += alphaIncrement)
    {
        double cur = collectorRadius * std::sin(alpha);
        double z = collectorRadius * std::cos(alpha);

        core::Vec3 xAxesOrigin(cur, 0, z);
        core::Vec3 yAxesOrigin(0, cur, z);

        if (std::abs(alpha - constants::kPi) < constants::kAccuracy)
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