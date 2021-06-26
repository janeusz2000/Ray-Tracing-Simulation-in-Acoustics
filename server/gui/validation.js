const chart = require("chart.js");
chart.defaults.scale.gridLines.color = "#ddd";

const currentIp = "http://localhost";
const currentPort = "3000";
const addressIp = currentIp + ":" + currentPort;
const numberOfValidationsModel = 8;

// TODO: refresh data every 5 min

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

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

async function renderComponents(validations, numberOfGraphsPerRow) {
  // used for drawing tables
  const ParameterNames = [
    "MEAN_ERROR",
    "STANDARD_DEVIATION_ERROR",
    "RMSE",
    "MAX_ERROR",
    "MIN_ERROR",
    "MEDIAN_ERROR",
  ];

  const currentComponentDiv = document.createElement("div");
  currentComponentDiv.style.setProperty("width", "100%");

  const propertyList = [...Array(numberOfValidationsModel).keys()].map(
    (number) => "SAMPLE" + (number + 1)
  );

  const statisticValuesIDPerSampleList = propertyList.map((property) =>
    validations.map((validation) => validation[property])
  );

  const statisticValues = await parseRequestsAndGetStatisticValuesFromDatabase(
    statisticValuesIDPerSampleList
  );

  const tempParameterName = ParameterNames[0];
  const newCanvas = document.createElement("canvas");
  newCanvas.id = "ValidationChart";
  const context = newCanvas.getContext("2d");
  currentComponentDiv.appendChild(newCanvas);

  const data = {};
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
      (table) => table[tempParameterName]
    );
    const color = sampleColor[sample];
    currentData.backgroundColor = currentData.data.map(
      (value) => "rgba(" + color + " 0.5)"
    );
    currentData.borderColor = currentData.data.map(
      (values) => "rgba(" + color + " 1)"
    );
    currentData.borderColor = 1;
    data.datasets.push(currentData);
  }

  const labels = [...Array(data.datasets[0].data.length).keys()].map(
    (value) => "Validation nr: " + value
  );
  data.labels = labels;

  console.log(data);

  const chart = new Chart(context, {
    type: "bar",
    data: data,
    options: {
      title: {
        display: true,
        fontColor: "white",
        text: tempParameterName,
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
    },
  });

  //   // generate new row every time x of the element
  //   // index in the table reach numberOfGraphsPerRow
  //   if (
  //     sample % numberOfGraphsPerRow == 0 &&
  //     sample != 0
  //   ) {
  //     currentRow = document.createElement("tr");
  //   }

  //   const currentCell = document.createElement("td");
  //   const newCanvas = document.createElement("canvas");
  //   newCanvas.id = "ValidationChart" + currentChart++;
  //   newCanvas.height = "300";
  //   newCanvas.width = "800";
  //   currentCell.appendChild(newCanvas);
  //   currentRow.appendChild(currentCell);
  //   if ((sample + 1) % numberOfGraphsPerRow == 0) {
  //     tableBody.appendChild(currentRow);
  //   }

  //   const context = newCanvas.getContext("2d");

  //   // DATA FOR CHARTS
  //
  //   const backgroundColors = statisticValues.map(
  //     (table) =>
  //       "rgba(" +
  //       getRandomInt(100, 255) +
  //       ", " +
  //       getRandomInt(100, 255) +
  //       ", " +
  //       getRandomInt(100, 255) +
  //       ", 0.5)"
  //   );
  //

  //   const borderColors = backgroundColors.map(
  //     (color) => color.substr(0, color.lastIndexOf(",") + 1) + "1)"
  //   );

  //   // const color = "#" + (((1 << 24) * Math.random()) | 0).toString(16);
  // }
  // currentTable.appendChild(tableBody);
  // currentComponentDiv.appendChild(currentTable);
  document.body.appendChild(currentComponentDiv);
}

// DRAWS every component on this site
function performRendering(numberOfGraphsPerRow) {
  getValidations().then((validation) =>
    renderComponents(validation, numberOfGraphsPerRow)
  );
}

const numberOfGraphsPerRow = 3;
performRendering(numberOfGraphsPerRow);
