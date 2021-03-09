'use strict';

import {RGB_ETC1_Format} from 'three';
import {EnergyCollector, getEnergyCollectors} from './src/simulation/addObjects'
const Chart = require('chart.js');

export class energyCollectorsArrayPerFrequency {
  constructor(energyCollector) { this.data = energyCollector.data; }
}

export function importEnergyCollectorsArrayPerFrequency() {
  return results.map(energyCollector => energyCollector.data);
}

const context = document.getElementById('myChart').getContext('2d');
const collectorData = importEnergyCollectorsArrayPerFrequency();
const energyCollectors = getEnergyCollectors();

class EnergyCollectorWithEnergy {
  constructor(EnergyCollector, energy) {
    this.x = EnergyCollector.x;
    this.y = EnergyCollector.y;
    this.z = EnergyCollector.z;
    this.energy = energy;
  }
}

export function associateCollectorWithData(energyCollectors, energyData) {
  const outputList = [];
  for (var index = 0; index < energyCollectors.length; index++) {
    outputList.push(new EnergyCollectorWithEnergy(energyCollectors[index],
                                                  energyData[index]));
  }
  return outputList;
}

export function divideCollectorListIntoTwoPolarPatters(
    collectorWithEnergyList) {

  const outputListEvenIndex = [];
  const outputListOddIndex = [];
  const listSize = collectorWithEnergyList.length;

  var index = listSize % 2;
  if (index == 1) {
    outputListEvenIndex.push(collectorWithEnergyList[0]);
    outputListOddIndex.push(collectorWithEnergyList[0]);
  }
  for (; index < listSize; index++) {
    if (index % 2 == 0) {
      outputListEvenIndex.push(collectorWithEnergyList[index]);
    } else {
      outputListOddIndex.push(collectorWithEnergyList[index]);
    }
  }

  return [ outputListEvenIndex, outputListOddIndex ];
}

export function compareCollectorsWithEnergy(collectorA, collectorB) {
  if (collectorA.x < collectorB.x) {
    return -1;
  } else if (collectorA.x > collectorB.x) {
    return 1;
  } else {
    if (collectorA.y < collectorB.y) {
      return -1;
    } else if (collectorA.y > collectorB.y) {
      return 1;
    }
  }
  return 0;
}

export function linSpaceArray(start, stop, n) {
  const outputList = [];
  var currentNumber = start;
  const step = Math.abs(start - stop) / n;
  if (start > stop) {
    step *= -1;
  }
  for (var index = 0; index < n; index++) {
    outputList.push(currentNumber);
    currentNumber += step;
  }
  return outputList;
}

// Refractor this because its ugly C:
export function drawPolarPattern() {
  var frequency = 0;
  const collectorsWithEnergyList =
      associateCollectorWithData(energyCollectors, collectorData[frequency])
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

  console.log(tempA);
  const tempB = energiesB.map(energy => {
    if (energy == 0) {
      return 0;
    } else {
      return 20 * Math.log10(energy)
    }
  });

  const MaxA = Math.max.apply(Math, tempA);
  console.log(MaxA);
  const MaxB = Math.max.apply(Math, tempA);

  const resultA = tempA.map(energy => energy - MaxA);
  const resultB = tempB.map(energy => energy - MaxB);

  while (resultA.length < angles.length) {
    resultA.push(-Infinity);
    resultB.unshift(-Infinity);
  }

  Chart.defaults.global.defaultFontColor = 'white';
  var index = 0;
  const chart = new Chart(context, {
    data : {
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
    },

    type : 'radar',
    options : {
      rotation : -Math.PI,
      circumference : Math.PI,
      scales : {
        r : {
          angleLines : {
            display : false,
          }
        }
      },
      scale : {
        gridLines : {
          color : "#9e9e9e",
        },
      },

      legend : {
        labels : {
          fontColor : "black",
        },
      },

    },

  });
}

drawPolarPattern();