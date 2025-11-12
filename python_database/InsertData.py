'''
this file will handle inserting data into the database
'''

import sqlite3
import main as m

def data_insert():
    # insert data into the table
    BooksData = [
        ('The First Programmer', 'Andy Hunt', 1999),
        ('Head First Python', 'Paul Barry', 2010),
        ('Clean Code', 'Robert C. Martin', 2008),
        ('The Pragmatic Programmer', 'Andrew Hunt and David Thomas', 1999),
        ('Introduction to Algorithms', 'Thomas H. Cormen', 2009),
    ]
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

    m.connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)
    print('Data insertion complete.')

    m.connection.commit()