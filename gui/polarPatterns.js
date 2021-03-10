'use strict';

import {
  addFrequencySliderForPolarPatterns,
} from './src/polarPatterns/addFrequencySliderForPolarPatterns';
import {getEnergyCollectors} from './src/simulation/getEnergyCollectors';
import {
  importEnergyCollectorsArrayPerFrequency
} from './src/polarPatterns/importEnergyCollectorsArrayPerFrequency'

const Chart = require('chart.js');
const context = document.getElementById('myChart').getContext('2d');
// Refractor this because its ugly C:
export function drawPolarPattern() {
  Chart.defaults.global.defaultFontColor = 'white';
  window.myChart = new Chart(context, {
    data : data,
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
        ticks : {
          suggestedMax : 0,
        }
      },

      legend : {
        labels : {
          fontColor : "black",
        },
      },

    },
  });
}

const collectorData = importEnergyCollectorsArrayPerFrequency();
const energyCollectors = getEnergyCollectors();
drawPolarPattern();
addFrequencySliderForPolarPatterns(collectorData, energyCollectors);