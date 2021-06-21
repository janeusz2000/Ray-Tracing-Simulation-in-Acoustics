import mysql.connector
from mysql.connector import MySQLConnection, Error
import numpy as np
from numpy.lib.function_base import insert
from numpy.lib.utils import source
from absl import app, flags
from ipConfig import local_ip

import logging

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


def insert_SimulationProperties(sourcePower: float, numOfCollectors: int, numOfRaysSquared: int):
    try:
        conn = mysql.connector.connect(
            host=local_ip, database='Validations', user='root', password='')

        if conn.is_connected():
            query = f'INSERT INTO SIMULATION_PROPERTIES(SOURCE_POWER, NUM_OF_COLLECTORS, TOTAL_NUMBER_OF_RAYS) VALUES ({sourcePower},{numOfCollectors}, {np.square(numOfRaysSquared)});'
            cursor = conn.cursor()
            cursor.execute(query)
            conn.commit()
            print(
                f"Data acquired in database: source power: {sourcePower} [W], number of collectors: {numOfCollectors}, Total number of Rays: {np.square(numOfRaysSquared)}")
    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()


def main(argv):
    sourcePower = float(argv[1])
    numOfCollectors = int(argv[2])
    numOfRaysSquared = int(argv[3])
    insert_SimulationProperties(sourcePower=sourcePower,
                                numOfCollectors=numOfCollectors,
                                numOfRaysSquared=numOfRaysSquared)
    logging.info("==== SIMULATION PROPERTIES INFO SAVED IN DATABASE ====")


if __name__ == '__main__':
    app.run(main)
