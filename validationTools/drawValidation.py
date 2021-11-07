import json
from mysql.connector import connect, Error
from ipConfig import local_ip
import matplotlib.pyplot as plt
import logging
import numpy as np

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)
ADDITIONAL_LABELS=[21, 25, 29, 33, 37]

def getValueFromDecimal(value):
    return float(value)


def getData(dataType: str, parameter: str, restriction: str):
    results = list()
    try:
        conn = connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            query = f"""
SELECT {dataType}, MODEL, STATISTIC_VALUES.PARAMETER_NAME, VALIDATION_ID FROM VALIDATION_DATA
LEFT JOIN STATISTIC_VALUES ON SAMPLE1 = STATISTIC_VALUES_ID
OR SAMPLE2 = STATISTIC_VALUES_ID OR SAMPLE3 = STATISTIC_VALUES_ID
OR SAMPLE4 = STATISTIC_VALUES_ID OR SAMPLE5 = STATISTIC_VALUES_ID
OR SAMPLE6 = STATISTIC_VALUES_ID OR SAMPLE7 = STATISTIC_VALUES_ID
OR SAMPLE8 = STATISTIC_VALUES_ID OR SAMPLE9 = STATISTIC_VALUES_ID
OR SAMPLE10 = STATISTIC_VALUES_ID
WHERE STATISTIC_VALUES.PARAMETER_NAME='{parameter}' AND VALIDATION_DESC='{restriction}'
ORDER BY VALIDATION_ID ASC, MODEL ASC
"""
            logging.info(f"executing query: {query}")
            cursor = conn.cursor()
            cursor.execute(query)
            results = cursor.fetchall()
    except Error as e:
        print(e)
    finally:
        if conn is not None and conn.is_connected():
            conn.close()
    returnTable = list()
    currentValidation = dict()
    for result in results:
        if result[1] not in currentValidation:
            currentValidation[result[1]] = {"statisticParameter": dataType, "value": getValueFromDecimal(
                result[0]), "parameterName": parameter, "validationNumber": result[3]}
        else:
            returnTable.append(currentValidation)
            currentValidation = dict()
            currentValidation[result[1]] = {"statisticParameter": dataType, "value": getValueFromDecimal(
                result[0]), "parameterName": parameter, "validationNumber": result[3]}
    returnTable.append(currentValidation)
    logging.info(f"Acquired table\n{json.dumps(returnTable, indent=3)}\n")
    return returnTable


def drawSinglePlot(data: list, ax, graphIndex):
    labels = [key for key in data[0]]
    name = data[0]["Analytical line Model A"]["parameterName"]
    statisticParameter = data[0]["Analytical line Model A"]["statisticParameter"]
    values = list()
    for label in labels:
        values.append([data[index][label]['value']
                       for index in range(len(data))])

    x = np.arange(len(values[0]))  # Label locations
    width = 0.18  # width of the bars

    graphs = list()
    for index, label in enumerate(labels):
        bar = ax[graphIndex].bar(x + index*width/2, values[index],
                                 width/2, label=label)
        graphs.append(bar)

    ax[graphIndex].set_ylabel(f"{name} [-]", fontsize=16)
    ax[graphIndex].set_title(statisticParameter, fontsize=18)
    ax[graphIndex].set_xticks(x+(len(labels)/2*width)/2 - width/4)
    ax[graphIndex].set_xticklabels(
            [f"Validation Nr {index}\nNum Of Collectors: {ADDITIONAL_LABELS[index]}" for index in x], fontsize=16)
    ax[graphIndex].tick_params(axis='y', labelsize=16)
    ax[graphIndex].legend()
    ax[graphIndex].yaxis.grid(linestyle='--')


def plotData(acousticParameter: str, description: str):
    parameterList = ["MEAN_ERROR",
                     "STANDARD_DEVIATION_ERROR", "RMSE"]
    fig, ax = plt.subplots(len(parameterList), figsize=(20, 24))
    for index, name in enumerate(parameterList):
        result = getData(name, acousticParameter, restriction=description)
        drawSinglePlot(result, ax, index)

    fig.tight_layout()
    plt.savefig(
        "./validationTools/diffusionReferenceGraphs/test.png", format='png')


def getTable(acousticParameter: str, restriction: str):
    result = list()
    try:
        conn = connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            query = f"""
SELECT MODEL, VALIDATION_ID, MEAN_ERROR, STANDARD_DEVIATION_ERROR, RMSE, MEDIAN_ERROR FROM VALIDATION_DATA
LEFT JOIN STATISTIC_VALUES ON SAMPLE1 = STATISTIC_VALUES_ID
OR SAMPLE2 = STATISTIC_VALUES_ID OR SAMPLE3 = STATISTIC_VALUES_ID
OR SAMPLE4 = STATISTIC_VALUES_ID OR SAMPLE5 = STATISTIC_VALUES_ID
OR SAMPLE6 = STATISTIC_VALUES_ID OR SAMPLE7 = STATISTIC_VALUES_ID
OR SAMPLE8 = STATISTIC_VALUES_ID OR SAMPLE9 = STATISTIC_VALUES_ID
OR SAMPLE10 = STATISTIC_VALUES_ID
WHERE STATISTIC_VALUES.PARAMETER_NAME='{acousticParameter}' AND VALIDATION_DESC='{restriction}'
ORDER BY VALIDATION_ID ASC, MODEL ASC
"""
            logging.info(f"executing query: {query}")
            cursor = conn.cursor()
            cursor.execute(query)
            results = cursor.fetchall()
    except Error as e:
        print(e)
    finally:
        if conn is not None and conn.is_connected():
            conn.close()

    output = list()
    currentValidation = None
    validation = dict()
    for result in results:
        if currentValidation is None or result[1] != currentValidation:
            if validation:
                output.append(validation)
            validation = dict()
            currentValidation = result[1]
        if result[0] not in validation:
            validation[result[0]] = {
                "Mean Error": getValueFromDecimal(result[2]),
                "Standard Deviation": getValueFromDecimal(result[3]),
                "Rmse": getValueFromDecimal(result[4]),
                "Median": getValueFromDecimal(result[5])}
        else:
            raise Error("WTF")

    output.append(validation)
    legend = "Nr Walidacji,Model Walidacyjny,Wartość Średnia błędu,Odchylenie Standardowe Błędu,Rmse,Mediana błędu"
    print(legend)
    logging.info(legend)
    for index, validation in enumerate(output):
        for model, values in validation.items():
            data = f"{index+1},{model},{values['Mean Error']},{values['Standard Deviation']},{values['Rmse']},{values['Median']}"
            print(data)
            logging.info(data)


if __name__ == "__main__":
    parameter = "Normalized Acoustic Diffusion Coefficient"
    restriction = "numOfCollectors2"
    plotData(parameter, restriction)
    getTable(parameter, restriction)
