const currentIp = "http://localhost";
const currentPort = "3000";
const addressIp = currentIp + ":" + currentPort;

const testDiv = document.createElement("div");
const testButton = document.createElement("button");
const testTextBox = document.createElement("input");
testTextBox.type = "text";

testDiv.appendChild(testButton);
testDiv.appendChild(testTextBox);
document.body.appendChild(testDiv);

const headers = new Headers();
headers.append("Content-Type", "application/json");
headers.append("Accept", "application/json");
headers.append("Access-Control-Allow-Origin", "http://localhost:3000");
headers.append("Access-Control-Allow-Credentials", "true");
headers.append("GET", "POST", "OPTIONS");

testButton.onclick = function () {
  fetch(addressIp + "/app/getValidationsID")
    .then((response) => console.log(response.json()))
    .catch((error) => console.log(error));
};

testButton.onclick();
