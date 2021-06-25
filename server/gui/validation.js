const currentIp = "http://localhost";
const currentPort = "3000";
const addressIp = currentIp + ":" + currentPort;
const numberOfValidationsModel = 8;

// TODO: refresh data every 5 min
const testDiv = document.createElement("testDiv");
testDiv.classList.add("testDiv");
const testButton = document.createElement("button");
testButton.classList.add("testButton");
testButton.innerHTML = "TEST BUTTON MAKE TEST QUERY";
testButton.onclick = function () {
  fetch(addressIp + "/app/getStatisticValues/17")
    .then((response) => console.log(response.json()))
    .catch((error) => console.log(error));
};
testDiv.appendChild(testButton);
document.body.appendChild(testDiv);

function renderSpaceForCharts(numberOfGraphsPerRow) {
  // DRAW OVERALL COMPONENT OF THE SITE
  for (
    var currentModel = 0;
    currentModel < numberOfValidationsModel;
    currentModel++
  ) {
    const currentComponentDiv = document.createElement("div");
    const ParameterNames = [
      "MEAN_ERROR",
      "STANDARD_DEVIATION_ERROR",
      "RMSE",
      "MAX_ERROR",
      "MIN_ERROR",
      "MEDIAN_ERROR",
    ];

    // DRAW TABLE
    for (
      var currentElementIndex = 0;
      currentElementIndex < ParameterNames.length;
      currentElementIndex++
    ) {
      console.log("Drawing element of the table");
    }

    // const chartCanvas = document.createElement("canvas");
    // chartCanvas.id = "ValidationChart" + currentModel;

    currentChartDiv.appendChild(chartCanvas);
    document.body.appendChild(currentComponentDiv);
    // DRAW TABLE COMPONENT:

    // testButton.onclick = function () {
    //   fetch(addressIp + "/app/getValidation")
    //     .then((response) => console.log(response.json()))
    //     .catch((error) => console.log(error));
    // };
  }
}

const numberOfGraphsPerRow = 3;
renderSpaceForCharts(numberOfGraphsPerRow);

// const headers = new Headers();
// headers.append("Content-Type", "application/json");
// headers.append("Accept", "application/json");
// headers.append("Access-Control-Allow-Origin", "http://localhost:3000");
// headers.append("Access-Control-Allow-Credentials", "true");
// headers.append("GET", "POST", "OPTIONS");
