'use strict';

import {addEnergyCollectors} from './src/simulation/addEnergyCollectors';
import {addModel} from './src/simulation/addModel';
import {drawTracking} from './src/simulation/drawTracking';
import {deleteTracking} from './src/simulation/deleteTracking';

import {animate} from './src/simulation/animate';
import {prepareScene} from './src/simulation/prepareScene';
import {addFrequencySlider} from './src/simulation/addFrequencySlider';

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

const arrowList = [];
const startingFrequency = addFrequencySlider(scene, arrowList);

window.addEventListener('resize', onWindowResize);
prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
addModel(scene);
drawTracking(scene, startingFrequency, arrowList)
animate(scene, renderer, camera, controls);
