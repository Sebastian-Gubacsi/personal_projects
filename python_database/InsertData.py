'''
this file will handle inserting data into the database
'''

import sqlite3
from main import connection, BooksData

# variables
length = len(BooksData)
print(f'Inserting {length} records into the database.')

# insert data into the table
BooksData.insert(length, ('Book title', 'test name', 2000))

connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)
print('Data insertion complete.')

connection.commit()