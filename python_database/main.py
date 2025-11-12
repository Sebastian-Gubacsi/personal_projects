"""
will support all logic and comunication with the sqlite databases and data manipulation
"""

import sqlite3
import InsertData as id
import CreateTable as ct
import CollectAndSortData as csd
'''
from Interface import
from InsertData import
from RemoveData import
from CreateDatabase import
from RetreiveData import
from SortData import
'''

database_name = input("Enter the database name: ")
connection = sqlite3.connect(database_name+'.db')

# create a table in the database
new_table = ct.create_table()

# connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)

# insert data into the table
new_insert = id.data_insert()

# retrieve and display data
print(csd.retrieve_data())