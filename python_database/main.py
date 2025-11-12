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

database_name = input("Enter the database name (with .db extension): ")
connection = sqlite3.connect(database_name)

# create a table in the database
new_table = ct.create_table()

# connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)

# insert data into the table
new_insert = id.data_insert()

print(csd.retrieve_data())  # retrieve and display data

'''
# query data from the table
result = connection.execute('SELECT * FROM books;')
data = result.fetchall()
print(data)

# display queried data
print('Books in Database:')
for row in data:
    print(f'Title: {row[1]}')
    print(f'Author: {row[2]}')
    print(f'Year: {row[3]}')
    print('')

# writes data to database
connection.commit()
'''

# closes the databases connection
# connection.close()