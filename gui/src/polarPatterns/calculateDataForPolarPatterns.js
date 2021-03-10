'use strict'

import{associateCollectorWithData} from './associateCollectorWithData'
import{divideCollectorListIntoTwoPolarPatters} from
    './divideCollectorListIntoTwoPolarPatters'

import{compareCollectorsWithEnergy} from './compareCollectorsWithEnergy'
import{linSpaceArray} from './linSpaceArray'

export function calculateDataForPolarPatterns(collectorData, energyCollectors,
                                              frequencyIndex) {
  const collectorsWithEnergyList = associateCollectorWithData(
      energyCollectors, collectorData[frequencyIndex])
  const outputLists =
      divideCollectorListIntoTwoPolarPatters(collectorsWithEnergyList);

  const dataA = outputLists[0];
  const dataB = outputLists[1];

  dataA.sort(compareCollectorsWithEnergy);
  dataB.sort(compareCollectorsWithEnergy);
  const angles = linSpaceArray(-180, 180, 2 * dataA.length);

  const energiesA =
      dataA.map(energyCollectorWithEnergy => energyCollectorWithEnergy.energy);
  const energiesB =
      dataB.map(energyCollectorWithEnergy => energyCollectorWithEnergy.energy);

  const tempA = energiesA.map(energy => {
    if (energy == 0) {
      return 0;
    } else {
      return 20 * Math.log10(energy)
    }
  });

  const tempB = energiesB.map(energy => {
    if (energy == 0) {
      return 0;
    } else {
      return 20 * Math.log10(energy)
    }
  });

  const MaxA = Math.max.apply(Math, tempA);
  const MaxB = Math.max.apply(Math, tempA);

  const resultA = tempA.map(energy => energy - MaxA);
  const resultB = tempB.map(energy => energy - MaxB);

  while (resultA.length < angles.length) {
    resultA.push(-Infinity);
    resultB.unshift(-Infinity);
  }
  return [ resultA, resultB, angles ]
}
