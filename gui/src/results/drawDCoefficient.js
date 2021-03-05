'use strict'
const Chart = require('chart.js')

export function drawDCoefficient(context, results, reference) {
  const frequencies = results.map(result => result.frequency);
  const dCoefficientValues = results.map(result => result.dCoefficient);
  const referenceD = reference.map(result => result.dCoefficient);

  const chart = new Chart(context, {
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
      elements : {
        line : {
          tension : 0,
        }
      },
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
        } ],

      }
    }
  });
}