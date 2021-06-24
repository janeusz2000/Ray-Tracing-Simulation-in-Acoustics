const path = require("path");
const destination = "server/dist";
const simulation = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/simulation.js",
  output: {
    filename: "simulation.js",
    path: path.resolve(__dirname, destination),
  },
};

const results = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/results.js",
  output: {
    filename: "results.js",
    path: path.resolve(__dirname, destination),
  },
};

const normalizedResults = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/normalizedResults.js",
  output: {
    filename: "normalizedResults.js",
    path: path.resolve(__dirname, destination),
  },
};

const preLoader = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/preLoader.js",
  output: {
    filename: "preLoader.js",
    path: path.resolve(__dirname, destination),
  },
};

const polarData = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/src/polarPatterns/data.js",
  output: {
    filename: "polarData.js",
    path: path.resolve(__dirname, destination),
  },
};

const polarPatterns = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/polarPatterns.js",
  output: {
    filename: "polarPatterns.js",
    path: path.resolve(__dirname),
  },
};

const validation = {
  mode: "development",
  devtool: false,
  entry: "./server/gui/validation.js",
  output: {
    filename: "validation.js",
    path: path.resolve(__dirname, destination),
  },
};

module.exports = [
  validation,
  polarData,
  polarPatterns,
  simulation,
  results,
  normalizedResults,
  preLoader,
];
