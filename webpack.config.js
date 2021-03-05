const path = require('path');

const simulation = {
  mode : 'development',
  devtool : false,
  entry : './gui/simulation.js',
  output : {filename : 'simulation.js', path : path.resolve(__dirname, 'dist')},
};

const results = {
  mode : 'development',
  devtool : false,
  entry : './gui/results.js',
  output : {filename : 'results.js', path : path.resolve(__dirname, 'dist')},
};

const normalizedResults = {
  mode : 'development',
  devtool : false,
  entry : './gui/normalizedResults.js',
  output : {
    filename : 'normalizedResults.js',
    path : path.resolve(__dirname, 'dist')
  },
};

module.exports = [ simulation, results, normalizedResults ];
