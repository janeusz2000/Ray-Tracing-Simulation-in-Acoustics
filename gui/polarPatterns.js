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
Chart.defaults.global.defaultFontColor = 'white';
var index = 0;
const chart = new Chart(context, {
  data : {
    labels : energyCollectors.map(energyCollector =>
                                      energyCollector.number.toString()),
    datasets : [
      {
        label : "EnergyCollectors",
        borderColor : 'rgb(255, 99, 132)',
        data : collectorData[index],
      },
    ]
  },

  type : 'radar',
  options : {
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
