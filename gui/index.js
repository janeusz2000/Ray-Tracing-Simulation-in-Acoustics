import {addObjects} from './addObjects';
import {prepareScene} from './prepareScene';

function animate(scene, renderer, camera, controls) {
  // Hold the current object's scope,
  // for accessing properties from within the callback method.
  var $this = this;
  function renderLoop() {
    requestAnimationFrame(renderLoop);
    controls.update(); // only required if controls.enableDamping = true, or if
                       // controls.autoRotate = true
    renderer.render(scene, camera);
  }
  renderLoop();
}

var THREE = require('three');
var OrbitControls = require('three-orbit-controls')(THREE);
var scene = new THREE.Scene();
var renderer = new THREE.WebGLRenderer({antialias : true});
var camera = new THREE.PerspectiveCamera(
    60, window.innerWidth / window.innerHeight, 1, 100);
var controls = new OrbitControls(camera, renderer.domElement);

prepareScene(scene, renderer, camera, controls);
addObjects(scene);

animate(scene, renderer, camera, controls);
