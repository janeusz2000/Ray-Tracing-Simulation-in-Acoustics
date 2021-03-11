'use strict';

import {addEnergyCollectors} from './src/simulation/addEnergyCollectors';
import {addModel} from './src/simulation/addModel';
import {drawTracking} from './src/simulation/drawTracking';
import {getModel} from './src/simulation/getModel';

import {animate} from './src/simulation/animate';
import {prepareScene} from './src/simulation/prepareScene';
import {addFrequencySlider} from './src/simulation/addFrequencySlider';
import {
  addToggleModelSwitchOnModelVsReference
} from './src/simulation/addToggleSwitchOnModelVsReference'
import {getTracking} from './src/simulation/getTracking';

class ActiveObjectTracker {
  constructor() {
    this.object = [];
    this.legend = [];
  }

  addObject(object, objectDescription) {
    this.object.push(object);
    this.legend.push(objectDescription);
  }

  getObject(objectDescription) {
    var index = -1;
    this.legend.find(element => {
      index++;
      return element === objectDescription;
    });
    return this.object[index];
  }
}

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
const objectTracker = new ActiveObjectTracker;
const arrowList = [];
const triangleList = [];

addToggleModelSwitchOnModelVsReference(scene, triangleList, objectTracker);
const startingFrequency = addFrequencySlider(scene, arrowList, objectTracker);

window.addEventListener('resize', onWindowResize);
prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
const unprocessedTriangles = getModel();
const tempTracking = getTracking();

addModel(scene, unprocessedTriangles, triangleList);
drawTracking(scene, startingFrequency, arrowList, tempTracking)
animate(scene, renderer, camera, controls);
