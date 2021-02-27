'use strict';
import {
  addEnergyCollectors,
  addModel,
  getTracking,
  drawTracking,
  deleteTracking
} from './addObjects';
import {animate} from './animate';
import {prepareScene} from './prepareScene';

function addFrequencySlider(scene, arrowList) {
  const frequencies =
      trackingData.map(dataPerFrequency => dataPerFrequency.frequency);
  const newDiv = document.createElement("div")

  const newOutput = document.createElement("output");
  newOutput.className = "FrequencyDisplay";
  newOutput.id = "FrequencyDisplay";
  newOutput.value = "Current Frequency: " + frequencies[0].toString() + " [Hz]";
  newDiv.appendChild(newOutput);
  document.body.appendChild(newDiv);

  const newSlider = document.createElement("input");
  newSlider.className = "FrequencySlider";
  newSlider.id = "FrequencySlider";
  newSlider.type = 'range';
  newSlider.value = 0;
  newSlider.min = 0;
  newSlider.max = frequencies.length - 1;
  newSlider.step = 1;
  newDiv.appendChild(newSlider);

  newSlider.addEventListener("input", function() {
    const newFrequencyValue = frequencies[newSlider.value];
    if (newFrequencyValue > 1000) {
      const outputFrequency = newFrequencyValue / 1000;
      newOutput.value =
          "Current frequency: " + outputFrequency.toString() + " [kHz]";
    } else {
      newOutput.value =
          "Current frequency: " + frequencies[newSlider.value].toString() +
          " [Hz]";
    }
    deleteTracking(scene, arrowList);
    drawTracking(scene, newFrequencyValue, arrowList);
  })
  return frequencies[0];
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

const arrowList = [];
const startingFrequency = addFrequencySlider(scene, arrowList);
window.addEventListener('resize', onWindowResize);
prepareScene(scene, renderer, camera, controls);
addEnergyCollectors(scene);
addModel(scene);
drawTracking(scene, startingFrequency, arrowList)
animate(scene, renderer, camera, controls);
