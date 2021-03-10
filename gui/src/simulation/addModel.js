'use strict';

import {getModel} from './getModel'
import {makeTriangle} from './makeTriangle'

export function addModel(scene) {
  const triangles = getModel();
  triangles.forEach(triangle => makeTriangle(triangle.point1, triangle.point2,
                                             triangle.point3, scene));
}
