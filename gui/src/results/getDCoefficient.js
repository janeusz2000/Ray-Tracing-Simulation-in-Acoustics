"use strict";

class Sample {
  constructor(time, energy) {
    this.time = time;
    this.energy = energy;
  }
}

function compareSamples(sampleA, sampleB) {
  if (sampleA.time < sampleB.time) {
    return -1;
  } else if (sampleA.time > sampleB) {
    return 1;
  }
  return 0;
}

function findSmallestTimeStep(timeArray) {
  var previousTime = 0;
  var smallestTimeStep = Number.MAX_SAFE_INTEGER;
  timeArray.forEach(timeValue => {
    const tempStep = Math.abs(timeValue - previousTime);
    if (tempStep < smallestTimeStep) {
      smallestTimeStep = tempStep;
    }
    previousTime = timeValue;
  })
  return smallestTimeStep;
}

function arrange(start, stop, step) {
  const size = Math.round(Math.abs(stop - start) / step);
  console.log(size)
  const output = [];
  var index = -1;
  output.forEach(value => {
    ++index;
    return index * step;
  })
  return output;
}

function dot(arrayA, arrayB) {
  if (arrayA.length != arrayB.length) {
    throw "Arrays in dot() have different sizes! ArrayA: " +
        arrayA.length.toString() + ", ArrayB: " + arrayB.length.toString();
  }
  var accumulator = 0;
  for (var index = 0; index < arrayA; ++index)
    accumulator += arrayA[index] * arrayB[index];

  return accumulator
}

export function getDCoefficient(data) {

  const energyCollectorData = data.map(collector => {
    const time = collector.time;
    const energy = collector.energy;
    if (energy.length == 0) {
      return 0;
    }
    const samples = [];
    for (var timeIndex = 0; timeIndex < time.length; ++timeIndex) {
      samples.push(new Sample(time[timeIndex], energy[timeIndex]));
    }
    samples.sort(compareSamples);

    // TODO: calculate time vector here
    // TODO: sum energies that are really close to each other.

    const energiesArray = samples.map(sample => sample.energy);

    return Math.sqrt(dot(energiesArray, energiesArray) /
                     energiesArray.length); // RMS
  });

  const energy = energyCollectorData.map(rmsValue => {
    if (rmsValue == 0 || rmsValue == NaN) {
      return 0;
    } else {
      return 120 + 10 * Math.log10(rmsValue);
    }
  })

  const alpha = Math.pow(energy.reduce((acc, curr) => acc + curr), 2);
  const beta = energy.reduce((acc, curr) => Math.pow(curr, 2) + acc);

  const gamma = (energy.length - 1) * beta;

  const output = (alpha - beta) / gamma;

  return output;
}