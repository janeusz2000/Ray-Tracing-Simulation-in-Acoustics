import mysql.connector
from mysql.connector import MySQLConnection, Error
from validationTools.ipConfig import local_ip
import logging
import json

from validationTools.compareResultsToReference import prepareParamterMap

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


def loadExampleReferenceData():
    PATH = "validationResults/Reference/line/1D_1m_modulo23_500Hz_46n_30stopni_5potega.json"
    with open(PATH) as jsonFile:
        reference = json.load(jsonFile)
        return prepareParamterMap(reference)


def getResponse(conn: mysql.connector.MySQLConnection):
    exampleReferenceData = loadExampleReferenceData()
    for parameterName in exampleReferenceData.keys():
        myCursor = conn.cursor()
        query = f"SELECT * FROM STATISTIC_VALUES WHERE " \
            f"STATISTIC_VALUES.PARAMETER_NAME='{parameterName}' " \
            f"ORDER BY STATISTIC_VALUES_ID DESC LIMIT 10; "
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
            "SIMULATION_PROPERTIES_ID, SAMPLE1, SAMPLE2, SAMPLE3, SAMPLE4, " \
            "SAMPLE5, SAMPLE6, SAMPLE7, SAMPLE8, SAMPLE9, SAMPLE10," \
            "VALIDATION_DESC) "

    queryValues = f"VALUES ('{parameterName}', {lastSimulationID}, " \
        f"{sampleIndexList[0]}, {sampleIndexList[1]}, {sampleIndexList[2]}, " \
        f"{sampleIndexList[3]}, {sampleIndexList[4]}, {sampleIndexList[5]}, " \
        f"{sampleIndexList[6]}, {sampleIndexList[7]}, {sampleIndexList[8]}, " \
        f"{sampleIndexList[9]}, \'{validationDesc}\');"

    return queryHeader + queryValues


def performLogging(desc: str):

    try:
        conn = mysql.connector.connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            for parameterName, response in getResponse(conn):
                sampleIndexList = [currentResponse[0]
                                   for currentResponse in reversed(response)]
                print(sampleIndexList)
                # divide samples into line validaion and surface validation
                sampleLineIndex = sampleIndexList[0::2]
                sampleSurfaceIndex = sampleIndexList[1::2]
                print("line: {}".format(sampleLineIndex))
                print("surface: {}".format(sampleSurfaceIndex))
                resultSampleIndexList = sampleLineIndex + sampleSurfaceIndex
                simulationID = getCurrentSimulationProperties(conn)
                query = prepareQuery(parameterName=parameterName,
                                     sampleIndexList=resultSampleIndexList,
                                     lastSimulationID=simulationID,
                                     validationDesc=desc)
                print(query)
                myCursor = conn.cursor()
                myCursor.execute(query)

                conn.commit()
                logging.info("==== VALIDATION SAVED IN DATABASE ==== ")

    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()
