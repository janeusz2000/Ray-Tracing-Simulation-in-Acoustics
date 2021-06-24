'use strict'

import{calculateDataForPolarPatterns} from './calculateDataForPolarPatterns'
import{getEnergyCollectors} from '../simulation/getEnergyCollectors';
import {
  importEnergyCollectorsArrayPerFrequency
} from './importEnergyCollectorsArrayPerFrequency'

const collectorData = importEnergyCollectorsArrayPerFrequency();
const energyCollectors = getEnergyCollectors();

const frequencyIndex = 0;
const result = calculateDataForPolarPatterns(collectorData, energyCollectors,
                                             frequencyIndex);
const resultA = result[0];
const resultB = result[1];
const angles = result[2];

data = {
  labels : angles.map(angle => angle.toString() + "'"),
  datasets : [
    {
      label : "Polar pattern circumference A",
      borderColor : 'rgb(255, 99, 132)',
      data : resultA,
    },
    {
      label : "Polar pattern circumference B",
      borderColor : 'rgb(0, 153, 0)',
      data : resultB,
    },
  ]
};