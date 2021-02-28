'use strict';

const Chart = require('chart.js')

const ctx = document.getElementById('myChart').getContext('2d');

export class Result {
  constructor(result) {
    this.frequency = result.frequency;
    this.dCoefficient = getDCoefficient(result.data);
  }
}

export class Reference {
  constructor(result) {
    this.frequency = result.frequency;
    this.dCoefficient = result.d;
  }
}

function compareResults(objectA, objectB) {
  if (objectA.frequency < objectB.frequency) {
    return -1;
  }
  if (objectA.frequency > objectB.frequency) {
    return 1;
  }
  return 0;
}

export function loadResults() {
  const output = results.map(result => new Result(result));
  output.sort(compareResults);
  return output;
}

export function loadReference() {
  const reference = referenceData.map(result => new Reference(result));
  reference.sort(compareResults);
  return reference;
}

// |data| is exported from energyCollector
export function getDCoefficient(data) {
  // TODO: Time Factor

  const energy = [];
  for (var energyIndex = 0; energyIndex < data.length; energyIndex++) {
    if (data[energyIndex] == 0) {
      energy.push(0);
    } else {
      energy.push(120 + 10 * Math.log10(Math.abs(data[energyIndex])));
    }
  }

  const alpha = Math.pow(energy.reduce((acc, curr) => acc + curr), 2);
  const beta = energy.reduce((acc, curr) => Math.pow(curr, 2) + acc);

  const gamma = (energy.length - 1) * beta;
  return (alpha - beta) / gamma
}

export function drawDCoefficient() {
  const results = loadResults();
  const reference = loadReference();
  const referenceFrequencies = reference.map(result => result.frequency);
  const frequencies = results.map(result => result.frequency);
  const dCoefficientValues = results.map(result => result.dCoefficient);
  const referenceD = reference.map(result => result.dCoefficient);
  console.log(referenceD);

  const chart = new Chart(ctx, {
    type : 'line',

    data : {
      labels : frequencies,
      datasets : [
        {
          label : 'Simulated Acoustic D Coefficient',
          borderColor : 'rgb(255, 99, 132)',
          data : dCoefficientValues,
        },
        {
          label : 'Reference Acoustic D Coefficient',
          borderColor : 'rgb(0, 153, 51)',
          data : referenceD,
        },
      ]
    },

    options : {
      plugins : {
        datalabels : {
          backgroundColor : function(
              context) { return context.dataset.backgroundColor; },
          borderRadius : 4,
          color : 'white',
          font : {weight : 'bold'},
          formatter : Math.round,
          padding : 6
        }
      },
      legend : {
        labels : {
          fontColor : "white",
        },
      },
      scales : {
        yAxes : [ {
          display : true,
          ticks : {
            suggestedMin : 0,
            suggestedMax : 1,
            fontColor : "white",
          },
          scaleLabel : {
            display : true,
            labelString : "Acoustic Diffusion Coefficient Value [-]",
            fontColor : "white",
          },
          gridLines : {
            color : "#9e9e9e",
          }
        } ],
        xAxes : [ {
          display : true,
          ticks : {
            display : true,
            fontColor : "white",
          },
          scaleLabel : {
            display : true,
            labelString : "Frequency [Hz]",
            fontColor : "white",
          },
          gridLines : {
            color : "#9e9e9e",
          }
        } ]

      }
    }
  });
}
drawDCoefficient();