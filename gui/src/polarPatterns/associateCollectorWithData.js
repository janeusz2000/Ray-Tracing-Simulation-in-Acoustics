'use strict'

import{EnergyCollectorWithEnergy} from './energyCollectorsWithEnergy'

export function associateCollectorWithData(energyCollectors, energyData) {
  const outputList = [];
  for (var index = 0; index < energyCollectors.length; index++) {
    outputList.push(new EnergyCollectorWithEnergy(energyCollectors[index],
                                                  energyData[index]));
  }
  return outputList;
}