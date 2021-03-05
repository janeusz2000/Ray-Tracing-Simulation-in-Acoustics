'use strict';

import {
  loadResults,
  loadReferencePlate,
  loadActualNormalizedDCoefficient
} from '../gui/src/results/loadData';
import {drawDCoefficient} from './src/results/drawDCoefficient';

const context = document.getElementById('myChart').getContext('2d');

const actualNormalizedD = loadActualNormalizedDCoefficient()
const dCoefficientValues = loadResults();
const referencePlateResult = loadReferencePlate();

for (var index = 0; index < dCoefficientValues.length; index++) {
  const dCoefficient = (dCoefficientValues[index].dCoefficient -
                        referencePlateResult[index].dCoefficient) /
                       (1 - referencePlateResult[index].dCoefficient);

  dCoefficientValues[index].dCoefficient = dCoefficient;
}

drawDCoefficient(context, dCoefficientValues, actualNormalizedD);