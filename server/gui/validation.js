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

  var currentChart = 0;
  for (
    var currentModel = 0;
    currentModel < numberOfValidationsModel;
    currentModel++
  ) {
    const currentComponentDiv = document.createElement("div");
    currentComponentDiv.style.setProperty("width", "100%");
    const currentProperty = "SAMPLE" + (currentModel + 1);
    const currentStatisticValuesList = validations.map(
      (validations) => validations[currentProperty]
    );

    const requestURL =
      addressIp +
      "/app/getStatisticValues/" +
      encodeURI(currentStatisticValuesList.toString());

    const response = await fetch(requestURL);
    const statisticValues = await response.json();

    console.log(statisticValues);

    const currentTable = document.createElement("table");
    const tableBody = document.createElement("tbody");
    var currentRow = document.createElement("tr");
    // DRAWING TABLE
    for (
      var currentElementIndex = 0;
      currentElementIndex < ParameterNames.length;
      currentElementIndex++
    ) {
      // generate new row every time x of the element
      // index in the table reach numberOfGraphsPerRow
      if (
        currentElementIndex % numberOfGraphsPerRow == 0 &&
        currentElementIndex != 0
      ) {
        currentRow = document.createElement("tr");
      }

      const currentCell = document.createElement("td");
      const newCanvas = document.createElement("canvas");
      newCanvas.id = "ValidationChart" + currentChart++;
      newCanvas.height = "300";
      newCanvas.width = "800";
      currentCell.appendChild(newCanvas);
      currentRow.appendChild(currentCell);
      if ((currentElementIndex + 1) % numberOfGraphsPerRow == 0) {
        tableBody.appendChild(currentRow);
      }

      const context = newCanvas.getContext("2d");

      // DATA FOR CHARTS
      const values = statisticValues.map(
        (table) => table[ParameterNames[currentElementIndex]]
      );
      const backgroundColors = statisticValues.map(
        (table) =>
          "rgba(" +
          getRandomInt(100, 255) +
          ", " +
          getRandomInt(100, 255) +
          ", " +
          getRandomInt(100, 255) +
          ", 0.5)"
      );
      const labels = [...Array(values.length).keys()].map(
        (value) => "Validation number: " + value
      );

      const borderColors = backgroundColors.map(
        (color) => color.substr(0, color.lastIndexOf(",") + 1) + "1)"
      );

      const data = {
        labels: labels,
        datasets: [
          {
            label: ParameterNames[currentElementIndex],
            data: values,
            backgroundColor: backgroundColors,
            borderColor: borderColors,
            borderWidth: 1,
          },
        ],
      };

      const chart = new Chart(context, {
        type: "bar",
        data: data,
        options: {
          legend: {
            labels: {
              fontColor: "white",
            },
          },
          scales: {
            x: {
              grid: {
                offset: true,
              },
            },
            grid: {
              color: "#FFFFFF",
            },
          },
        },
      });

      // const randomColor = "#" + (((1 << 24) * Math.random()) | 0).toString(16);
    }
    currentTable.appendChild(tableBody);
    currentComponentDiv.appendChild(currentTable);
    document.body.appendChild(currentComponentDiv);
  }
}

// DRAWS every component on this site
function performRendering(numberOfGraphsPerRow) {
  getValidations().then((validation) =>
    renderComponents(validation, numberOfGraphsPerRow)
  );
}

const numberOfGraphsPerRow = 3;
performRendering(numberOfGraphsPerRow);
