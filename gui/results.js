"use strict";
import {EnergyCollector} from './addObjects';

const Chart = require('chart.js')

const ctx = document.getElementById('myChart').getContext('2d');
export class Result {
  constructor(result) {
    this.frequency = result.frequency;
    this.dCoefficient = getDCoefficient(result.data);
  }
}

export function loadResults() {
  return results.map(result => new Result(result));
}

// |data| is exported from energyCollector
export function getDCoefficient(data) {
  // TODO: Time Factor
  const energy = data.map(level => (120 + 10 * Math.log10(level)));
  const alpha = Math.pow(energy.reduce((acc, curr) => acc + curr), 2);
  const beta = energy.reduce((acc, curr) => Math.pow(acc, 2) + curr);

  const gamma = (energy.length - 1) * beta;
  return (alpha - beta) / gamma
}

export function drawDCoefficient() {
  const results = loadResults();
  const frequencies = results.map(result => (result.frequency));
  const dCoefficientValues = results.map(result => (result.dCoefficient));

  const chart = new Chart(ctx, {
    type : 'line',

    data : {
      labels : frequencies,
      datasets : [ {
        label : 'Simulated Acoustic D Coefficient',
        backgroundColor : 'rgb(255, 99, 132)',
        borderColor : 'rgb(255, 99, 132)',
        data : dCoefficientValues
      } ]
    },

    options : {}
  });
}
drawDCoefficient();