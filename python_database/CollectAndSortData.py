'''
this file will handle retrieving data from the database to display to an interface.
'''

import sqlite3
import InsertData as insert

def retrieve_data():
    # query data from the table
    result = insert.connection.execute('SELECT * FROM books;')
    data = result.fetchall()
    print(data)

    # display queried data
    print('Books in Database:')
    for row in data:
        print(f'Title: {row[1]}')
        print(f'Author: {row[2]}')
        print(f'Year: {row[3]}')
        print('')

    insert.connection.close()