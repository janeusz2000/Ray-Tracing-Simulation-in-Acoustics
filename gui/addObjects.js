import data from './data.json';
import {hsl} from './hsl';
import {makeInstance} from './makeInstance';

var THREE = require('three');

export class EnergyCollector {
  constructor(energyCollector) {
    this.number = energyCollector.number;
    this.x = energyCollector.x;
    this.z = energyCollector.z;
    this.y = energyCollector.y;
    this.radius = energyCollector.radius;
  }
}

export function makeCollector(number, x, y, z, radius, scene) {
  const geometry = new THREE.SphereGeometry(radius, 32, 32);
  const energyCollector = makeInstance(geometry, hsl(1 / 8, 1, .5), x, z, y);
  scene.add(energyCollector);
}

export function getEnergyCollectors() {
  return data.map(collector => new EnergyCollector(collector));
}

export function addEnergyCollectors(scene) {
  const energyCollectors = getEnergyCollectors();

  energyCollectors.forEach(energyCollector => makeCollector(
                               energyCollector.number, energyCollector.x,
                               energyCollector.y, energyCollector.z,
                               energyCollector.radius, scene));
}
