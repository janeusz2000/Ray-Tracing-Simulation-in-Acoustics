var THREE = require('three');
var OrbitControls = require('three-orbit-controls')(THREE);

let camera, controls, scene, renderer;

function hsl(h, s, l) { return (new THREE.Color()).setHSL(h, s, l); }

function makeInstance(geometry, color, x, y, z) {
  const material =
      new THREE.MeshPhongMaterial({color, opacity : 0.5, transparent : true});

  const cube = new THREE.Mesh(geometry, material);
  cube.position.set(x, y, z);
  scene.add(cube);

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
      60, window.innerWidth / window.innerHeight, 1, 100);
  camera.position.set(40, 20, 0);

  // controls

  controls = new OrbitControls(camera, renderer.domElement);
  //   controls.listenToKeyEvents(window); // optional

  // controls.addEventListener( 'change', render ); // call this only in static
  // scenes (i.e., if there is no animation loop)

  controls.enableDamping = true; // an animation loop is required when either
                                 // damping or auto-rotation are enabled
  controls.dampingFactor = 0.01;
  controls.screenSpacePanning = false;
  controls.minDistance = 1;
  controls.maxDistance = 20;
  controls.rotateSpeed = 0.01;
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
  const d = 0.8;
  makeInstance(geometry, hsl(0 / 8, 1, .5), -d, -d, -d);
  makeInstance(geometry, hsl(1 / 8, 1, .5), d, -d, -d);
  makeInstance(geometry, hsl(2 / 8, 1, .5), -d, d, -d);
  makeInstance(geometry, hsl(3 / 8, 1, .5), d, d, -d);
  makeInstance(geometry, hsl(4 / 8, 1, .5), -d, -d, d);
  makeInstance(geometry, hsl(5 / 8, 1, .5), d, -d, d);
  makeInstance(geometry, hsl(6 / 8, 1, .5), -d, d, d);
  makeInstance(geometry, hsl(7 / 8, 1, .5), d, d, d);

  // put your object here

  //   var fs = require('fs');
  //   var data = fs.readFileSync('data/data.json', 'utf8');
  //   var energycollectors = JSON.parse(energycollectors)

  window.addEventListener('resize', onWindowResize);
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
init();
animate();