const THREE = require("three");

export function prepareScene(scene, renderer, camera, controls) {
  // scene.background = new THREE.Color(0xFFFFFF);
  scene.background = new THREE.Color(0xffffff);
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);
  camera.position.set(0, 0, 20);
  controls.enableDamping = true; // an animation loop is required when either
  // damping or auto-rotation are enabled
  controls.dampingFactor = 0.01;
  controls.screenSpacePanning = false;
  controls.minDistance = 1;
  controls.maxDistance = 40;
  controls.rotateSpeed = 0.01;
  controls.maxPolarAngle = Math.PI / 2;

  const dirLight1 = new THREE.DirectionalLight(0xffffff);
  dirLight1.position.set(1, 1, 1);
  scene.add(dirLight1);

  const dirLight2 = new THREE.DirectionalLight(0x002288);
  dirLight2.position.set(-1, -1, -1);
  scene.add(dirLight2);

  const ambientLight = new THREE.AmbientLight(0x222222);
  scene.add(ambientLight);
}
