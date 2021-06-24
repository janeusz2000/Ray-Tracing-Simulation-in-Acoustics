'use strict';

import {Triangle} from './Triangle';

export function getModel() {
  return model.map(triangle => new Triangle(triangle));
}
