'use strict'

import{Triangle} from './Triangle';

export function getReferenceModel() {
  return referenceModel.map(triangle => new Triangle(triangle));
}