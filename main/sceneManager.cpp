#include "main/sceneManager.h"
SceneManager::SceneManager(int collectors, double simulationRadius) : numCollectors_(collectors), simulationRadius_(simulationRadius)
{
    if (numCollectors_ % 4 == 0 || (numCollectors_ - 1) % 4 == 0)
    {
        // ( 2 *  pi * R ) / ( 2 * population)  for dividing whole half circle distance to equal distances for population of energyCollectors
        collectorRadius_ = 2 * constants::kPi * simulationRadius_ / (2 * numCollectors_);
        createCollectors();
    }
    else
    {
        std::stringstream ss;
        ss << "collector population:" << numCollectors_ << " % 4 or population - 1: " << numCollectors_ - 1 << " % 4 must be zero";
        throw std::invalid_argument(ss.str().c_str());
    }
}
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
    energyCollectors_.reserve(numCollectors_);
    size_t numToGo;

    if (numCollectors_ % 2 == 0)
    {
        numToGo = numCollectors_;
    }
    else
    {
        energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(
            core::Vec3(0, 0, simulationRadius_ / 2),
            collectorRadius_));
        numToGo = numCollectors_ - 1;
    }
    std::vector<core::Vec3> origins;

    for (size_t iteration = 0; iteration * 4 < numToGo; ++iteration)
    {
        double groundCoordinate = simulationRadius_ / 2 * std::cos(2 * constants::kPi * iteration / numToGo);
        double zCoordinate = simulationRadius_ / 2 * std::sin(2 * constants::kPi * iteration / numToGo);

        origins = {
            core::Vec3(groundCoordinate, 0, zCoordinate),
            core::Vec3(0, -groundCoordinate, zCoordinate),
            core::Vec3(-groundCoordinate, 0, zCoordinate),
            core::Vec3(0, groundCoordinate, zCoordinate),
        };

        for (const core::Vec3 &origin : origins)
        {
            energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(origin, collectorRadius_));
        }
    }
}