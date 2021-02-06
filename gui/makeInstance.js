var THREE = require('three')

export function makeInstance(geometry, color, x, y, z) {
  const material =
      new THREE.MeshPhongMaterial({color, opacity : 0.5, transparent : true});

  const object = new THREE.Mesh(geometry, material);
  object.position.set(x, y, z);
  return object;
}