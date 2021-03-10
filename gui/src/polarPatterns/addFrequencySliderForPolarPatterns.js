'use strict'

import{calculateDataForPolarPatterns} from './calculateDataForPolarPatterns'

export function addFrequencySliderForPolarPatterns(collectorData,
                                                   energyCollectors) {
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
    const tempResult = calculateDataForPolarPatterns(
        collectorData, energyCollectors, newSlider.value);
    data.datasets[0].data = tempResult[0];
    data.datasets[1].data = tempResult[1];
    window.myChart.update();
  })

  return frequencies[0];
}