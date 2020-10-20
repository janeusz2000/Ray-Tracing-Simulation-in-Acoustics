#include "main/sceneManager.h"
SceneManager::SceneManager(size_t collectors, double simulationRadius) : numCollectors_(collectors), simulationRadius_(simulationRadius)
{
    if (collectors % 4 != 0 || collectors - 1 % 4 != 0)
    {
        std::stringstream ss;
        ss << "collector population:" << collectors << " % 4 or population - 1: " << collectors - 1 << " % 4 must be zero";
        throw std::invalid_argument(ss.str().c_str());
    }
    // ( 2 *  pi * R ) / 2  for dividing whole half circle distance to equal distances
    collectorRadius_ = constants::kPi * simulationRadius_ / numCollectors_;
    createCollectors();
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
    size_t evenNum;

    if (numCollectors_ % 2 == 0)
    {
        evenNum = numCollectors_;
    }
    else
    {
        evenNum = numCollectors_ - 1;
        energyCollectors_.push_back(std::make_unique<objects::EnergyCollector>(
            core::Vec3(0, 0, simulationRadius_ / 2),
            collectorRadius_));
    }
    std::vector<core::Vec3> origins;
    for (size_t iteration = 0; iteration * 4 < evenNum; ++iteration)
    {
        double groundCoordinate = simulationRadius_ / 2 * std::cos(constants::kPi * iteration / evenNum);
        double zCoordinate = simulationRadius_ / 2 * std::sin(constants::kPi * iteration / evenNum);

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