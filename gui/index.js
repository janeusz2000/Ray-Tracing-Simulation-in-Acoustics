'use strict';
import {addEnergyCollectors, addModel, getTracking} from './addObjects';
import {animate} from './animate';
import {prepareScene} from './prepareScene';

const THREE = require('three');
const OrbitControls = require('three-orbit-controls')(THREE);
const scene = new THREE.Scene();
const SimulationWindow = document.getElementById('SimulationWindow');
const renderer =
    new THREE.WebGLRenderer({antialias : true, canvas : SimulationWindow});

const camera = new THREE.PerspectiveCamera(
    60, window.innerWidth / window.innerHeight, 1, 100);

const controls = new OrbitControls(camera, renderer.domElement);

function onWindowResize() {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();

  renderer.setSize(window.innerWidth, window.innerHeight);
}
window.addEventListener('resize', onWindowResize);

prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
addModel(scene);
getTracking(scene);
animate(scene, renderer, camera, controls);
