const path = require('path');

const simulation = {
  mode : 'development',
  devtool : false,
  entry : './gui/index.js',
  output : {filename : 'main.js', path : path.resolve(__dirname, 'dist')},
};

const results = {
  mode : 'development',
  devtool : false,
  entry : './gui/results.js',
  output : {filename : 'results.js', path : path.resolve(__dirname, 'dist')},
};

module.exports = [ simulation, results ];
