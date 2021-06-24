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
  const output = [];
  var index = 0;
  while ((start + index * step) < stop) {
    output.push(start + index * step);
    ++index;
  }
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
    if (energy.length == 0 || time.length == 0) {
      return 0;
    }

    const samples = [];
    for (var timeIndex = 0; timeIndex < time.length; ++timeIndex) {
      samples.push(new Sample(time[timeIndex], energy[timeIndex]));
    }
    samples.sort(compareSamples);

    // const smallestTimeStep = findSmallestTimeStep(time);
    // const stop = Math.max.apply(Math, time);
    // const start = Math.min.apply(Math, time);
    // const timeArray = arrange(start, stop, smallestTimeStep);
    // const energyArray = new Array(timeArray.length).fill(0);
    // samples.forEach(sample => {
    //   const sampleIndex = parseInt(sample.time / smallestTimeStep);
    //   energyArray[sampleIndex] = sample.energy;
    // })

    const RMS = Math.sqrt(dot(energy, energy) / energy.length);
    return RMS;
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