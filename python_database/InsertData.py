'''
this file will handle inserting data into the database
'''

import sqlite3
from main import connection, BooksData, database_name

def insert_data():
    # variables
    count = 0
    length = len(BooksData)
    print(f'Inserting {length} records into the database.')

    # the max will be relpaced with a button to add all data into the database of choice at once
    max = int(input('How many records do you want to add? '))

    # insert data into the table
    while count < max:
        BooksData.insert(length + count, ('Book title', 'test name', 2000))
        count += 1

    connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)
    print('Data insertion complete.')

    connection.commit()