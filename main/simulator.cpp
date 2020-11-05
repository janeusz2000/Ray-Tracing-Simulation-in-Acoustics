#include "main/simulator.h"

std::vector<std::unique_ptr<objects::EnergyCollector>>
buildCollectors(const Model &model, int numCollectors) {

  float minRadius = std::max(model.height(), model.sideSize());

}