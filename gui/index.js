import {addEnergyCollectors, addModel} from './addObjects';
import {animate} from './animate';
import {prepareScene} from './prepareScene';

var THREE = require('three');
var OrbitControls = require('three-orbit-controls')(THREE);
var scene = new THREE.Scene();
var renderer = new THREE.WebGLRenderer({antialias : true});
var camera = new THREE.PerspectiveCamera(
    60, window.innerWidth / window.innerHeight, 1, 100);
var controls = new OrbitControls(camera, renderer.domElement);

prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
addModel(scene);
animate(scene, renderer, camera, controls);
