#include "main/sceneManager.h"

std::vector<objects::EnergyCollector *> SceneManager::getEnergyCollectors()
{
    std::vector<objects::EnergyCollector *> temp;
    temp.reserve(energyCollectors_.size());
    for (const auto &t : energyCollectors_)
    {
        temp.push_back(t.get());
    }
    return temp;
}

// PRIVATE METHODS

void SceneManager::createCollectors() // TODO: rewrite this, because it doesn't work. I messed uo with radians and degrees
{
    energyCollectors_.reserve(collectors_);

    const double collectorRadius = constants::kDefaultSimulationRadius / 2; // TODO: delete this dependency
    double alphaIncrement;

    if (constants::kPopulation % 2 == 0)
    {
        alphaIncrement = 2 * constants::kPi * 180 / (static_cast<double>(collectors_) / 2 * 360);
    }
    else
    {
        alphaIncrement = 2 * constants::kPi * 180 / ((static_cast<double>(collectors_) - 1) / 2 * 360);
    }

    for (double alpha = 0; alpha <= constants::kPi; alpha += alphaIncrement)
    {
        double groundCoordinate = collectorRadius * std::sin(alpha);
        double z = collectorRadius * std::cos(alpha);

        core::Vec3 xAxesOrigin(groundCoordinate, 0, z);
        core::Vec3 yAxesOrigin(0, groundCoordinate, z);

        if (xAxesOrigin == core::Vec3(0, 0, constants::kDefaultSimulationRadius / 2)) // TODO: delete  this dependency
        {
            energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(xAxesOrigin));
        }
        else
        {
            energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(xAxesOrigin));
            energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(yAxesOrigin));
        }
    }
}