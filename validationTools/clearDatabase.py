import logging
from mysql.connector import connect, Error
from ipConfig import local_ip


logging.basicConfig(filename="./validationRaport.log", level=logging.INFO)

try:
    conn = connect(host=local_ip, database='Validations',
                   user='Brad', password='')

    if conn.is_connected():
        with open('./validationTools/database/clearDatabaseProcedure.db', encoding='utf-8') as f:
            cursor = conn.cursor()
            file = f.read()
            cursor.execute(file)
            conn.commit()
            logging.info("=== DATABASE CLEAR SUCCESSFUL! ===")
except Error as e:
    print(e)
    logging.info("=== !!!DATABASE CLEAR NOT SUCCESSFUL!!! ===")

finally:
    if conn is not None and conn.is_connected():
        conn.close()
