import model from '../data/model.json';
import trackingData from '../data/trackingData.json';

import energyCollectors from './energyCollectors.json';
import {hsl} from './hsl';
import {makeInstance, makeInstance2D} from './makeInstance';

var THREE = require('three');

export class Vec3 {
  constructor(vector3) {
    this.x = vector3.x;
    this.y = vector3.y;
    this.z = vector3.z;
  }
}

export class Triangle {
  constructor(triangle) {
    this.point1 = new Vec3(triangle.point1);
    this.point2 = new Vec3(triangle.point2);
    this.point3 = new Vec3(triangle.point3);
  }
}
export function getModel() {
  return model.map(triangle => new Triangle(triangle));
}

export function makeTriangle(point1, point2, point3, scene) {

  const geometry = new THREE.BufferGeometry();
  const positions = [];

  positions.push(point1.x, point1.z, point1.y);
  positions.push(point2.x, point2.z, point2.y);
  positions.push(point3.x, point3.z, point3.y);

  geometry.setAttribute('position',
                        new THREE.Float32BufferAttribute(positions, 3));

  geometry.computeVertexNormals();

  const object = makeInstance2D(geometry, hsl(5 / 8, 1, .5), 1);
  scene.add(object);
}

export function addModel(scene) {
  const triangles = getModel();
  triangles.forEach(triangle => makeTriangle(triangle.point1, triangle.point2,
                                             triangle.point3, scene));
}

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
  const energyCollector =
      makeInstance(geometry, hsl(1 / 8, 1, .5), 0.3, x, z, y);
  scene.add(energyCollector);
}

export function getEnergyCollectors() {
  return energyCollectors.map(collector => new EnergyCollector(collector));
}

export function addEnergyCollectors(scene) {
  const collectors = getEnergyCollectors();

  collectors.forEach(energyCollector => makeCollector(
                         energyCollector.number, energyCollector.x,
                         energyCollector.y, energyCollector.z,
                         energyCollector.radius, scene));
}

export function getRandomColorAttribute() {
  const color = new THREE.Color(0xffffff);
  color.setHex(Math.random() * 0xffffff);
  return color;
}

export function getTracking(scene) {
  for (var index = 0; index < trackingData.length; index++) {
    const color = getRandomColorAttribute();

    var currentTracking = trackingData[index];
    for (var rayTrackIndex = 0; rayTrackIndex < currentTracking.length;
         rayTrackIndex++) {

      const currentRay = currentTracking[rayTrackIndex];

      const direction =
          new THREE.Vector3(currentRay.direction.x, currentRay.direction.z,
                            currentRay.direction.y);
      const origin = new THREE.Vector3(currentRay.origin.x, currentRay.origin.z,
                                       currentRay.origin.y);
      const length = currentRay.length;
      const arrow = new THREE.ArrowHelper(direction, origin, length, color);
      scene.add(arrow);
    }
  }
}