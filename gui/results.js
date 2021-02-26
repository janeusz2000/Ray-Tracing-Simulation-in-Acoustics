var Chart = require('chart.js')

export class Result {
  constructor(result) {
    this.frequency = result.frequency;

    var data = result.data;
  }
}

export function LoadResults() {
  return results.map(result => new Result(result));
}
