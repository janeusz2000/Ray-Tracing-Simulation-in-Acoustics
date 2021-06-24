'use strict';

export function deleteModelFromSimulation(scene, triangleList) {
  triangleList.forEach(triangle => { scene.remove(triangle); });
  triangleList = [];
}