const express = require("express");
const app = express();
const mysql = require("mysql");
const fetch = require("node-fetch");
const fs = require("fs");
const path = require("path");

class DatabaseConnection {
  constructor() {
    // Every time WSL is initiated on the machine, it changes IP address.
    // This short code below reads ip address from config file, where current
    // IP address is stored.
    const configFile = this.#getIPConfigFile();
    // Keyword which is next to IP address in the config file.
    this.addressIP = this.#parseConfigFile(configFile);
    console.log("Database Address IP acquired: " + this.addressIP);
    this.testConnection();
  }

  getLocalIp() {
    return this.addressIP;
  }

  getConnection() {
    return mysql.createConnection({
      host: this.addressIP,
      user: "Brad",
      password: "",
      database: "Validations",
    });
  }

  getValidationsIDsFromDatabase() {
    const newConnection = this.getConnection();
    return new Promise(function (resolve, reject) {
      const queryMessage =
        "SELECT * FROM VALIDATION_DATA ORDER BY VALIDATION_ID ASC";
      newConnection.query(queryMessage, function (error, rows, fields) {
        if (error) return reject(error);
        resolve(rows);
      });
    });
  }

  getStatisticValuesByIDFromDataBase(TableID) {
    const newConnection = this.getConnection();
    return new Promise(function (resolve, reject) {
      const queryMessage =
        "SELECT * FROM STATISTIC_VALUES WHERE STATISTIC_VALUES_ID IN (" +
        TableID +
        ");";
      newConnection.query(queryMessage, function (error, rows, fields) {
        if (error) return reject(error);
        resolve(rows);
      });
      newConnection.end();
    });
  }

  testConnection() {
    const newConnection = this.getConnection();
    newConnection.connect(function (error) {
      if (error) {
        console.log(
          "There was an error with database connection at: " + this.addressIP
        );
        throw error;
      }
    });
    const queryMessage = "SELECT VALIDATION_ID FROM VALIDATION_DATA;";
    newConnection.query(queryMessage, function (error, result, fields) {
      if (error) {
        console.log(
          "There was a problem with SELECT privileges of the user BRAD"
        );
        throw error;
      }
      console.log("Connection with Database OK!");
    });
    newConnection.end();
  }

  #parseConfigFile(configFile) {
    const keyWord = "nameserver";
    const IPIndexStart = configFile.indexOf(keyWord) + keyWord.length + 1;
    const IPIndexEnd = configFile.indexOf("\n", IPIndexStart);
    return configFile.substr(IPIndexStart, IPIndexEnd - IPIndexStart);
  }

  #getIPConfigFile() {
    return fs.readFileSync(
      "/run/resolvconf/resolv.conf",
      "utf-8",
      (error, data) => {
        if (error) throw error;
        return data;
      }
    );
  }
}

const databaseConnection = new DatabaseConnection();

app.use(function (req, res, next) {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader(
    "Access-Control-Allow-Methods",
    "GET, POST, OPTIONS, PUT, PATCH, DELETE"
  );
  res.setHeader(
    "Access-Control-Allow-Headers",
    "X-Requested-With,content-type"
  );
  res.setHeader("Access-Control-Allow-Credentials", true);

  next();
});

// DEFAULT SITE

// SIMULATION HTML REQUESTS
app.get("/", function (request, response) {
  response.sendFile(path.join(__dirname + "/AppCards/simulation.html"));
});

app.get("/simulation.html", function (request, response) {
  response.sendFile(path.join(__dirname + "/AppCards/simulation.html"));
});

// MENU
app.get("/validations.html", function (request, response) {
  response.sendFile(path.join(__dirname + "/AppCards/validations.html"));
});

app.get("/css/simulationStyle.css", function (request, response) {
  response.sendFile(path.join(__dirname + "/css/simulationStyle.css"));
});

app.get("/data/loading.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/data/loading.js"));
});

app.get("/gui/preLoaderCheckState.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/gui/preloaderCheckState.js"));
});

app.get("/data/model.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/data/model.js"));
});

app.get("/data/trackingData.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/data/trackingData.js"));
});

app.get("/data/energyCollectors.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/data/energyCollectors.js"));
});

app.get("/dist/simulation.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/dist/simulation.js"));
});

// VALIDATION REQUESTS
app.get("/dist/validation.js", function (request, response) {
  response.sendFile(path.join(__dirname + "/dist/validation.js"));
});

// VALIDATION INTERNAL REQUESTS
app.get("/app/getValidation", function (request, response) {
  response.setHeader("Content-type", "application/json");
  databaseConnection
    .getValidationsIDsFromDatabase()
    .then((data) => JSON.parse(JSON.stringify(data)))
    .then((data) => response.json(data))
    .catch((error) =>
      setImmediate(() => {
        throw error;
      })
    );
});

app.get("/app/getStatisticValues/*", function (request, response) {
  const statisticValuesTableID = request.originalUrl.substr(
    request.originalUrl.lastIndexOf("/") + 1
  );
  databaseConnection
    .getStatisticValuesByIDFromDataBase(statisticValuesTableID)
    .then(function (data) {
      if (data.length == 0) {
        throw Error(
          "Table Statistic Values" + statisticValueTableID + " doesn't exist!"
        );
      }
      return JSON.parse(JSON.stringify(data));
    })
    .then((data) => response.json(data))
    .catch((error) =>
      setImmediate(() => {
        throw error;
      })
    );
});

app.listen(3000, () => console.log("Listening on port 3000"));
