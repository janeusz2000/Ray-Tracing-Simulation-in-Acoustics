'use strict'
import{Result, Reference} from './result';

export function compareResults(objectA, objectB) {
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

export function loadActualNormalizedDCoefficient() {
  const reference =
      referenceNormalizedData.map(result => new Reference(result));
  reference.sort(compareResults);
  return reference;
}

export function loadReferencePlate() {
  const output =
      referenceResults.map(referenceResult => new Result(referenceResult));
  output.sort(compareResults);
  return output;
}
