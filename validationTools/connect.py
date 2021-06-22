import mysql.connector
from mysql.connector import MySQLConnection, Error
from ipConfig import local_ip
import logging

logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)


def main():
    conn = None
    try:
        conn = mysql.connector.connect(
            host=local_ip, database='Validations', user='Brad', password='')

        if conn.is_connected():
            logging.info("Connection Successful!")

            queryCheck = "SELECT * FROM VALIDATION_DATA;"
            cursor = conn.cursor()
            cursor.execute(queryCheck)
            response = cursor.fetchall()

            if response is not None:
                logging.info("Select operation Sucessfull!")
            else:
                logging.error("Select operation was not successful!")

    except Error as e:
        print(e)

    finally:
        if conn is not None and conn.is_connected():
            conn.close()


if __name__ == "__main__":
    main()
