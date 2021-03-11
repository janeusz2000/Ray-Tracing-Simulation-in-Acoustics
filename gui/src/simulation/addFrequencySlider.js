'use strict'
import{drawTracking} from './drawTracking'
import{deleteTracking} from './deleteTracking';
import {getReferenceTracking} from './getReferenceTracking';
import {getTracking} from './getTracking';

export function addFrequencySlider(scene, arrowList, objectTracker) {
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
  objectTracker.addObject(newSlider, "frequencySlider");
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

    var allTracking = [];
    if (objectTracker.getObject("modelToggleSwitch").checked) {
      allTracking = getReferenceTracking();
    } else {
      allTracking = getTracking();
    }

    drawTracking(scene, newFrequencyValue, arrowList, allTracking);
  })
  return frequencies[0];
}