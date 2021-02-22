var THREE = require('three')

export function makeInstance(geometry, color, opacity, x, y, z) {
  const material = new THREE.MeshPhongMaterial(
      {color, opacity : opacity, transparent : true, side : THREE.BackSide});

  const object = new THREE.Mesh(geometry, material);
  object.position.set(x, y, z);
  return object;
}

export function makeInstance2D(geometry, color, opacity) {
  const material = new THREE.MeshPhongMaterial(
      {color, opacity : opacity, transparent : true});

  const object = new THREE.Mesh(geometry, material);
  return object;
}
