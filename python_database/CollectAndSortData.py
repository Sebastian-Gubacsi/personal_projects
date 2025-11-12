'''
this file will handle retrieving data from the database to display to an interface.
'''

import main as m

def retrieve_data():
    # query data from the table
    result = m.connection.execute('SELECT * FROM books;')
    data = result.fetchall()

    # display queried data
    print('Books in Database:')
    for row in data:
        print(f'Title: {row[1]}')
        print(f'Author: {row[2]}')
        print(f'Year: {row[3]}')
        print('')

    m.connection.close()