'use strict';

const THREE = require('three');

export function getRandomColorAttribute() {
  const color = new THREE.Color(0xffffff);
  color.setHex(Math.random() * 0xffffff);
  return color;
}