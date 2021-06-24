import {getDCoefficient} from './getDCoefficient'

export class Result {
  constructor(result) {
    this.frequency = result.frequency;
    this.dCoefficient = getDCoefficient(result.data);
  };
}

export class Reference {
  constructor(result) {
    this.frequency = result.frequency;
    this.dCoefficient = result.d;
  }
}