'''
this file will handle retrieving data from the database to display to an interface.
'''

def retrieve_data(connection):
    """
    Retrieve all rows from the `books` table using the provided `connection`, print them,
    and return the fetched rows.

    Args:
        connection: sqlite3.Connection object

    Returns:
        list of tuples: rows from the books table
    """
    # query data from the table
    result = connection.execute('SELECT * FROM books;')
    data = result.fetchall()

    # display queried data
    print('Books in Database:')
    for row in data:
        print(f'Title: {row[1]}')
        print(f'Author: {row[2]}')
        print(f'Year: {row[3]}')
        print('')

    # do not close the connection here; let caller decide
    return data