import {hsl} from './hsl';
import {makeInstance} from './makeInstance';

var THREE = require('three')

export function addObjects(scene) {
  const geometry = new THREE.SphereGeometry(0.5, 32, 32)
  const d = 0.8;
  scene.add(makeInstance(geometry, hsl(0 / 8, 1, .5), -d, -d, -d));
  scene.add(makeInstance(geometry, hsl(1 / 8, 1, .5), d, -d, -d));
}
