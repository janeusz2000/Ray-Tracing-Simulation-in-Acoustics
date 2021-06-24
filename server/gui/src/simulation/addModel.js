'use strict';

import {makeTriangle} from './makeTriangle'

export function addModel(scene, triangles, triangleList) {
  triangles.forEach(triangle =>
                        makeTriangle(triangle.point1, triangle.point2,
                                     triangle.point3, scene, triangleList));
}
