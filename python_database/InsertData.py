'''
this file will handle inserting data into the database
'''

def data_insert(connection):
    """
    Insert sample book data into the provided sqlite3 `connection`.

    Args:
        connection: sqlite3.Connection object
    """
    '''
    # insert data into the table
    BooksData = [
        ('The First Programmer', 'Andy Hunt', 1999),
        ('Head First Python', 'Paul Barry', 2010),
        ('Clean Code', 'Robert C. Martin', 2008),
        ('The Pragmatic Programmer', 'Andrew Hunt and David Thomas', 1999),
        ('Introduction to Algorithms', 'Thomas H. Cormen', 2009),
    ]
    '''

    # variables
    count = 0
    BooksData = []
    length = len(BooksData)
    print(f'Inserting {length} records into the database.')

    # the max will be replaced with a button to add all data into the database of choice at once
    try:
        max_records = int(input('How many records do you want to add? '))
    except ValueError:
        print('Invalid number entered, defaulting to 0 additional records.')
        max_records = 0

    # insert placeholder records if user requested more
    while count < max_records:
        BooksData.insert(length + count, ('Book title', 'test name', 2000))
        count += 1

    connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)
    print('Data insertion complete.')

    connection.commit()