const chart = require("chart.js");
chart.defaults.scale.gridLines.color = "#ddd";

const currentIp = "http://localhost";
const currentPort = "3000";
const addressIp = currentIp + ":" + currentPort;
const numberOfValidationsModel = 8;
const propertyList = [...Array(numberOfValidationsModel).keys()].map(
  (number) => "SAMPLE" + (number + 1)
);

var validationChart;

const ParameterNames = [
  "MEAN_ERROR",
  "STANDARD_DEVIATION_ERROR",
  "RMSE",
  "MAX_ERROR",
  "MIN_ERROR",
  "MEDIAN_ERROR",
];

async function getValidations() {
  const response = await fetch(addressIp + "/app/getValidation");
  const validation = await response.json();
  return validation;
}

async function parseRequestsAndGetStatisticValuesFromDatabase(requests) {
  const outputArray = [];
  for (var requestId = 0; requestId < requests.length; requestId++) {
    const requestURL =
      addressIp + "/app/getStatisticValues/" + encodeURI(requests[requestId]);
    const response = await fetch(requestURL);
    const statisticValuesList = await response.json();
    outputArray.push(statisticValuesList);
  }
  return outputArray;
}

function getData(parameterName, statisticValues) {
  data = {};
  data.datasets = [];
  sampleColor = [
    "255, 45, 0,",
    "244, 255, 0,",
    "24, 255, 0,",
    "0, 242, 255,",
    "0, 17, 255,",
    "241, 0, 255,",
    "0, 0, 0,",
    "132, 0, 255,",
  ];
  for (var sample = 0; sample < statisticValues.length; sample++) {
    const currentData = {};
    currentData.label = "SAMPLE" + (sample + 1);
    currentData.data = statisticValues[sample].map(
      (table) => table[parameterName]
    );
    const color = sampleColor[sample];
    currentData.backgroundColor = currentData.data.map(
      () => "rgba(" + color + " 0.5)"
    );
    currentData.borderColor = currentData.data.map(
      () => "rgba(" + color + " 1)"
    );
    currentData.borderColor = 1;
    data.datasets.push(currentData);
  }

  labels = [...Array(data.datasets[0].data.length).keys()].map(
    (value) => "Validation nr: " + value
  );
  data.labels = labels;
  return data;
}

async function renderSimulationPropertiesTable(validations) {
  console.log(validations);
  f;
}

async function renderComponents(validations) {
  // used for drawing tables

  const currentComponentDiv = document.createElement("div");
  currentComponentDiv.style.setProperty("width", "98%");

  const statisticValuesIDPerSampleList = propertyList.map((property) =>
    validations.map((validation) => validation[property])
  );

  const statisticValues = await parseRequestsAndGetStatisticValuesFromDatabase(
    statisticValuesIDPerSampleList
  );

  currentParameterName = ParameterNames[0];
  const newCanvas = document.createElement("canvas");
  newCanvas.id = "ValidationChart";
  const context = newCanvas.getContext("2d");
  currentComponentDiv.appendChild(newCanvas);

  const data = getData("MEAN_ERROR", statisticValues);

  console.log(data);

  const options = {
    title: {
      display: true,
      fontColor: "white",
      text: currentParameterName,
    },
    legend: {
      labels: {
        fontColor: "white",
      },
    },
    scales: {
      yAxes: [
        {
          ticks: {
            fontColor: "white",
          },
        },
      ],
      xAxes: [
        {
          ticks: {
            fontColor: "white",
          },
        },
      ],
      x: {
        grid: {
          offset: true,
        },
        grid: {
          color: "#FFFFFF",
        },
      },
    },
  };

  validationChart = new Chart(context, {
    type: "bar",
    data: data,
    options: options,
  });

  document.body.appendChild(currentComponentDiv);

  renderSimulationPropertiesTable(validations);
}

// DRAWS every component on this site
async function performRendering(numberOfGraphsPerRow) {
  getValidations().then((validation) =>
    renderComponents(validation, numberOfGraphsPerRow)
  );

  const parameterButtons = ParameterNames.map((parameterName) =>
    document.getElementById(parameterName.toLowerCase() + "_button")
  );

  parameterButtons.forEach((button) =>
    button.addEventListener("click", async function () {
      const buttonParameterName = button.id
        .substr(0, button.id.lastIndexOf("_"))
        .toUpperCase();

      const validations = await getValidations();

      const statisticValuesIDPerSampleList = propertyList.map((property) =>
        validations.map((validation) => validation[property])
      );

      const statisticValues =
        await parseRequestsAndGetStatisticValuesFromDatabase(
          statisticValuesIDPerSampleList
        );

      validationChart.data = getData(buttonParameterName, statisticValues);

      validationChart.options.title.text = buttonParameterName;
      validationChart.update();
    })
  );
}

performRendering();
