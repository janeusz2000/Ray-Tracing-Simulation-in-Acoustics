import mysql.connector
from mysql.connector import MySQLConnection, Error
import numpy as np
from validationTools.ipConfig import local_ip

import logging

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


def insert_statistic_Values(
        parameterName: str,
        modelName: str,
        meanError: float,
        standardDeviation: float,
        rmse: float,
        maxError: float,
        minError: float,
        medianError: float):

    try:
        conn = mysql.connector.connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            query = f"INSERT INTO STATISTIC_VALUES(PARAMETER_NAME, MODEL, MEAN_ERROR, STANDARD_DEVIATION_ERROR, RMSE, MAX_ERROR, MIN_ERROR, MEDIAN_ERROR) VALUES ('{parameterName}', '{modelName}', {meanError}, {standardDeviation}, {rmse}, {maxError}, {minError}, {medianError})"
            logging.info(query)
            cursor = conn.cursor()
            cursor.execute(query)
            conn.commit()
            logging.info("==== STATISTICS SAVED IN DATABASE ====")

    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()
