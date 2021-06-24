'use strict';
import {loadResults, loadReference} from '../gui/src/results/loadData';
import {drawDCoefficient} from './src/results/drawDCoefficient';

const context = document.getElementById('myChart').getContext('2d');
const results = loadResults();
const reference = loadReference();
drawDCoefficient(context, results, reference);