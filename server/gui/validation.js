const chart = require("chart.js");
const { traceDeprecation } = require("process");
chart.defaults.scale.gridLines.color = "#ddd";

Element.prototype.remove = function () {
  this.parentElement.removeChild(this);
};

NodeList.prototype.remove = HTMLCollection.prototype.remove = function () {
  for (var i = this.length() - 1; i >= 0; i--) {
    if (this[i] && this[i].parentElement) {
      this[i].parentElement.removeChild(this[i]);
    }
  }
};

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

async function requestSimulationPropertiesByIDs(simulationPropertiesIds) {
  const requestURL =
    addressIp +
    "/app/getSimulationProperties/" +
    encodeURI(simulationPropertiesIds);
  const response = await fetch(requestURL);
  const statisticProperties = await response.json();

  return statisticProperties;
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

async function drawInterface(validations) {
  const validation = validations[0];
  const simulationID = validation.SIMULATION_PROPERTIES_ID;
  const simulationPropertiesArray = await requestSimulationPropertiesByIDs(
    simulationID
  );
  const simulationProperties = simulationPropertiesArray[0];
  const row = document.createElement("tr");

  const validationID = document.createElement("th");
  validationID.id = "ValidationIDHeader";
  validationID.innerHTML = "Validation nr: ";
  validationID.className = "tableHeader";
  row.appendChild(validationID);

  for (const propertyName in simulationProperties) {
    const cell = document.createElement("th");
    cell.id = propertyName + "Header";
    cell.innerHTML = propertyName;
    cell.className = "tableHeader";
    row.appendChild(cell);
  }

  const desc = document.createElement("th");
  desc.id = "descHeader";
  desc.innerHTML = "Description";
  desc.className = "tableHeader";
  row.appendChild(desc);
  return row;
}

async function renderSimulationPropertiesTable(validations) {
  // console.log(validations);
  const currentDiv = document.createElement("div");
  currentDiv.id = "tableSimulationProperties";
  currentDiv.className = "tableContent";

  const table = document.createElement("table");
  const tableBody = document.createElement("tbody");
  const headers = await drawInterface(validations);
  tableBody.appendChild(headers);

  for (const validationID in validations) {
    const validation = validations[validationID];
    const simulationId = validation.SIMULATION_PROPERTIES_ID;
    const simulationPropertiesArray = await requestSimulationPropertiesByIDs(
      simulationId
    );
    const simulationProperties = simulationPropertiesArray[0];
    const row = document.createElement("tr");

    const validationIDCell = document.createElement("td");
    validationIDCell.id = "ValidationNumber";
    validationIDCell.innerHTML = (validation.VALIDATION_ID - 1).toString();
    validationIDCell.className = "dataCell";
    row.appendChild(validationIDCell);

    for (const propertyName in simulationProperties) {
      const cell = document.createElement("td");
      cell.id = propertyName;
      cell.innerHTML = simulationProperties[propertyName];
      cell.className = "dataCell";
      row.appendChild(cell);
    }

    const descriptionCell = document.createElement("td");
    descriptionCell.id = "descriptionCell" + validation.VALIDATION_ID;
    descriptionCell.innerHTML = validation.VALIDATION_DESC;
    descriptionCell.className = "dataCell";
    row.appendChild(descriptionCell);

    tableBody.appendChild(row);
  }

  table.appendChild(tableBody);
  currentDiv.appendChild(table);
  document.body.appendChild(currentDiv);
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
          scaleLabel: {
            display: true,
            labelString: "Value [-]",
            fontColor: "white",
          },
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

      const tableDiv = document
        .getElementById("tableSimulationProperties")
        .remove();
      renderSimulationPropertiesTable(validations);
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
