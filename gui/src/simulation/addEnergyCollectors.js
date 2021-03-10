'use strict'

import{makeCollector} from './makeCollector'
import{getEnergyCollectors} from './getEnergyCollectors'

export function addEnergyCollectors(scene) {
  const collectors = getEnergyCollectors();

  collectors.forEach(energyCollector => makeCollector(
                         energyCollector.number, energyCollector.x,
                         energyCollector.y, energyCollector.z,
                         energyCollector.radius, scene));
}