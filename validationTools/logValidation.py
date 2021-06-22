import mysql.connector
from mysql.connector import MySQLConnection, Error
import numpy as np
from ipConfig import local_ip
import logging
import json
from absl import app, flags

from compareResutlsToReference import prepareParamterMap

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


def loadExampleReferenceData():
    PATH = './validationResults/Reference/1D_0.5m_modulo7_2000Hz'\
        '_10n_60stopni_kwadratPlusJeden.json'
    with open(PATH) as jsonFile:
        reference = json.load(jsonFile)
        return prepareParamterMap(reference)


def getResponse(conn: mysql.connector.MySQLConnection):
    exampleReferenceData = loadExampleReferenceData()
    for parameterName in exampleReferenceData.keys():
        myCursor = conn.cursor()
        query = f"SELECT * FROM STATISTIC_VALUES WHERE " \
            f"STATISTIC_VALUES.PARAMETER_NAME='{parameterName}' " \
            f"ORDER BY STATISTIC_VALUES_ID DESC LIMIT 8; "
        myCursor.execute(query)
        yield (parameterName, myCursor.fetchall())


def getCurrentSimulationProperties(conn: mysql.connector.MySQLConnection):
    myCursor = conn.cursor()
    query = f"SELECT SIMULATION_ID FROM SIMULATION_PROPERTIES ORDER BY " \
        "SIMULATION_ID DESC LIMIT 1;"
    myCursor.execute(query)
    return myCursor.fetchall()[0][0]


def prepareQuery(parameterName: str,
                 sampleIndexList: list,
                 lastSimulationID: int,
                 validationDesc: str):

    queryHeader = "INSERT INTO VALIDATION_DATA(PARAMETER_NAME, " \
        "SIMULATION_PROPERTIES_ID, SAMPLE1, SAMPLE2, SAMPLE3, SAMPLE4," \
        "SAMPLE5, SAMPLE6, SAMPLE7, SAMPLE8, VALIDATION_DESC) "

    queryValues = f"VALUES ('{parameterName}', {lastSimulationID}, " \
        f"{sampleIndexList[0]}, {sampleIndexList[1]}, {sampleIndexList[2]}, " \
        f"{sampleIndexList[3]}, {sampleIndexList[4]}, {sampleIndexList[5]}, " \
        f"{sampleIndexList[6]}, {sampleIndexList[7]}, '{validationDesc}');"

    return queryHeader + queryValues


def execute(argv):
    desc = ""
    for i in range(1, len(argv)):
        desc += argv[i] + " "
    print(desc)

    try:
        conn = mysql.connector.connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            for parameterName, response in getResponse(conn):
                sampleIndexList = [currentResponse[0]
                                   for currentResponse in reversed(response)]
                simulationID = getCurrentSimulationProperties(conn)
                query = prepareQuery(parameterName=parameterName,
                                     sampleIndexList=sampleIndexList,
                                     lastSimulationID=simulationID,
                                     validationDesc=desc)
                myCursor = conn.cursor()
                myCursor.execute(query)
                conn.commit()
                logging.info("==== VALIDATION SAVED IN DATABASE ==== " +
                             "query:  " + query)

    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()


if __name__ == "__main__":
    app.run(execute)
