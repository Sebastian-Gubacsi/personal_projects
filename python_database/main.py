"""
will support all logic and comunication with the sqlite databases and data manipulation
"""

import sqlite3
import InsertData as id
import CreateTable as ct
import CollectAndSortData as csd
import RemoveData as rd
'''
from Interface import
from RemoveData import
from CreateDatabase import
'''

database_name = input("Enter the database name: ")
connection = sqlite3.connect(database_name +'.db')

# create a table in the database (pass the connection to avoid circular imports)
ct.create_table(connection)

# insert data into the table
id.data_insert(connection)

# retrieve and display data
csd.retrieve_data(connection)

# remove data from the table based on ID input
rd.remove_data(connection)