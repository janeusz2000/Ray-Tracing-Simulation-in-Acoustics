'use strict';

import {makeInstance2D} from './makeInstance';
import {getRandomColorAttribute} from './getRandomColorAttribute';

const THREE = require('three');

export function makeTriangle(point1, point2, point3, scene, triangleList) {

  const geometry = new THREE.BufferGeometry();
  const positions = [];

  positions.push(point1.x, point1.z, point1.y);
  positions.push(point2.x, point2.z, point2.y);
  positions.push(point3.x, point3.z, point3.y);

  geometry.setAttribute('position',
                        new THREE.Float32BufferAttribute(positions, 3));

  geometry.computeVertexNormals();

  const object = makeInstance2D(geometry, getRandomColorAttribute(), 1);
  scene.add(object);
  triangleList.push(object);
}