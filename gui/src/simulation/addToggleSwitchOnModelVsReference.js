'use strict';
import {addModel} from './addModel';
import {deleteModelFromSimulation} from './deleteModelFromSimulation'
import {getModel} from './getModel';
import {getReferenceModel} from './getReference';

export function addToggleModelSwitchOnModelVsReference(scene, triangleList,
                                                       objectTracker) {
  const newDiv = document.createElement("div");
  newDiv.className = "ToggleSwitch";
  const newLabel = document.createElement("label")
  newLabel.className = "switch";

  const newInput = document.createElement("input");
  newInput.type = "checkbox";
  objectTracker.addObject(newInput, "modelToggleSwitch");
  newLabel.appendChild(newInput);

  const newSpan = document.createElement("span")
  newSpan.className = "slider round";
  newLabel.appendChild(newSpan);

  newDiv.appendChild(newLabel);

  const newOutput = document.createElement("output");
  newOutput.className = "modelIndicator";
  newOutput.value = "Model";
  newDiv.appendChild(newOutput);

  newInput.addEventListener("change", function() {
    if (newInput.checked) {
      newOutput.value = "Reference Model"
      deleteModelFromSimulation(scene, triangleList);
      const tempTriangles = getReferenceModel();
      addModel(scene, tempTriangles, triangleList);
      const event = new Event("input");
      objectTracker.getObject("frequencySlider").dispatchEvent(event);

    } else {
      newOutput.value = "Model";
      deleteModelFromSimulation(scene, triangleList);
      const tempTriangles = getModel();
      addModel(scene, tempTriangles, triangleList);
      const event = new Event("input");
      objectTracker.getObject("frequencySlider").dispatchEvent(event);
    }
  })

  document.body.appendChild(newDiv);
}
