"""
will support all logic and comunication with the sqlite databases and data manipulation
"""

import sqlite3
import os
import InsertData as id
import CreateTable as ct
import CollectAndSortData as csd
import RemoveData as rd
'''
from Interface import
from RemoveData import
from CreateDatabase import
'''

# variables and initial setup
databases_dir = 'databases'
display = True
db_files = [f.replace('.db', '') for f in os.listdir(databases_dir) if f.endswith('.db')]

# Create databases folder if it doesn't exist
if not os.path.exists(databases_dir):
    os.makedirs(databases_dir)

for i in range(len(db_files)):
    print(f"{i + 1}: {db_files[i]}")


pos = int(input("Which database do you want to enter: "))
db_path = os.path.join(databases_dir, db_files[pos - 1] +'.db')
connection = sqlite3.connect(db_path)

# create a table in the database
ct.create_table(connection)

# insert data into the table
id.data_insert(connection)
while display == True:
    # retrieve and display data
    csd.retrieve_data(connection)

    # remove data from the table based on ID input
    rd.remove_data(connection)
    again = input('Do you want to view and remove more books? (y/n): ')
    if again.lower() != 'y':
        display = False

connection.close()