const THREE = require('three');

export function hsl(h, s, l) { return (new THREE.Color()).setHSL(h, s, l); }