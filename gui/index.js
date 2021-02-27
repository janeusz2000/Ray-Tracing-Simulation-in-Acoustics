'use strict';
import {addEnergyCollectors, addModel, getTracking} from './addObjects';
import {animate} from './animate';
import {prepareScene} from './prepareScene';

const THREE = require('three');
const OrbitControls = require('three-orbit-controls')(THREE);
const scene = new THREE.Scene();
const renderer = new THREE.WebGLRenderer({antialias : true});
const camera = new THREE.PerspectiveCamera(
    60, window.innerWidth / window.innerHeight, 1, 100);
const controls = new OrbitControls(camera, renderer.domElement);

prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
addModel(scene);
getTracking(scene);
animate(scene, renderer, camera, controls);

// TODO: dCoefficient caluclation per frequency
