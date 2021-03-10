'use strict'

import{Vec3} from './vec3';

export class Triangle {
  constructor(triangle) {
    this.point1 = new Vec3(triangle.point1);
    this.point2 = new Vec3(triangle.point2);
    this.point3 = new Vec3(triangle.point3);
  }
}