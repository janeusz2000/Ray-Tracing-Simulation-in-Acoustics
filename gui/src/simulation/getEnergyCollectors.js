'use strict'

import{EnergyCollector} from './EnergyCollector'

export function getEnergyCollectors() {
  return energyCollectors.map(collector => new EnergyCollector(collector));
}