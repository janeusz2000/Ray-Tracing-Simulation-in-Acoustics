import mysql.connector
from mysql.connector import MySQLConnection, Error
import numpy as np
import validationTools.ipConfig as ip


def logSimulationProperties(simulationProperties):
    try:
        conn = mysql.connector.connect(
            host=ip.local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            query = f'INSERT INTO SIMULATION_PROPERTIES(SOURCE_POWER, NUM_OF_COLLECTORS, TOTAL_NUMBER_OF_RAYS, MAX_TRACKING) VALUES ({simulationProperties.sourcePower},{simulationProperties.numOfCollectors}, {np.square(simulationProperties.numOfRaysSquared)}, {simulationProperties.maxTracking});'
            cursor = conn.cursor()
            cursor.execute(query)
            conn.commit()
            print(
                f"Data acquired in database: source power: {simulationProperties.sourcePower} [W], number of collectors: {simulationProperties.numOfCollectors}, Total number of Rays: {np.square(simulationProperties.numOfRaysSquared)}")
    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()
