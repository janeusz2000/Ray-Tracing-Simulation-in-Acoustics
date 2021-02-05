import {OrbitControls} from '../node_modules/three/build/OrbitControls.js';
// import * as THREE from '../node_modules/three/build/three.module.js';

const THREE = require('../node_modules/three/build/three.module.js')

let camera, controls, scene, renderer;

init();
// render(); // remove when using next line for animation loop
// (requestAnimationFrame)
animate();

function hsl(h, s, l) { return (new THREE.Color()).setHSL(h, s, l); }

function makeInstance(geometry, color, x, y, z) {
  const material =
      new THREE.MeshPhongMaterial({color, opacity : 0.5, transparent : true});

  const cube = new THREE.Mesh(geometry, material);
  scene.add(cube);

  cube.position.x = x;
  cube.position.set(x, y, z);

  return cube;
}

function init() {
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xcccccc);

  renderer = new THREE.WebGLRenderer({antialias : true});
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);

  camera = new THREE.PerspectiveCamera(
      60, window.innerWidth / window.innerHeight, 1, 1000);
  camera.position.set(40, 20, 0);

  // controls

  controls = new OrbitControls(camera, renderer.domElement);
  controls.listenToKeyEvents(window); // optional

  // controls.addEventListener( 'change', render ); // call this only in static
  // scenes (i.e., if there is no animation loop)

  controls.enableDamping = true; // an animation loop is required when either
                                 // damping or auto-rotation are enabled
  controls.dampingFactor = 0.05;
  controls.screenSpacePanning = false;
  controls.minDistance = 1;
  controls.maxDistance = 100;
  controls.maxPolarAngle = Math.PI / 2;

  // lights

  const dirLight1 = new THREE.DirectionalLight(0xffffff);
  dirLight1.position.set(1, 1, 1);
  scene.add(dirLight1);

  const dirLight2 = new THREE.DirectionalLight(0x002288);
  dirLight2.position.set(-1, -1, -1);
  scene.add(dirLight2);

  const ambientLight = new THREE.AmbientLight(0x222222);
  scene.add(ambientLight);

  // world

  const geometry = new THREE.SphereGeometry(0.5, 32, 32)

  //   var fs = require('../tests/testData/data.json');
  //   //   var data = fs.readFileSync('../../tests/testData/data.json',
  //   'utf8');

  //   window.addEventListener('resize', onWindowResize);
}

function onWindowResize() {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();

  renderer.setSize(window.innerWidth, window.innerHeight);
}

function animate() {
  requestAnimationFrame(animate);

  controls.update(); // only required if controls.enableDamping = true, or if
                     // controls.autoRotate = true

  render();
}

function render() { renderer.render(scene, camera); }
