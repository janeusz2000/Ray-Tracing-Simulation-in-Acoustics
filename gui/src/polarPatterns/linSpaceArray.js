'use strict'

export function linSpaceArray(start, stop, n) {
  const outputList = [];
  var currentNumber = start;
  const step = Math.abs(start - stop) / n;
  if (start > stop) {
    step *= -1;
  }
  for (var index = 0; index < n; index++) {
    outputList.push(currentNumber);
    currentNumber += step;
  }
  return outputList;
}