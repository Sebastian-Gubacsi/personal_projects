'''
this file will handle removing data from the database
'''

def remove_data(connection):
    """
    Remove books from the `books` table based on user input.

    Args:
        connection: sqlite3.Connection object
    """
    try:
        book_id = int(input('Enter the ID of the book to remove (or 0 to skip): '))
        if book_id > 0:
            connection.execute('DELETE FROM books WHERE id = ?;', (book_id,))
            connection.commit()
            print(f'Book with ID {book_id} has been removed.')
        else:
            print('No books were removed.')
    except ValueError:
        print('Invalid ID entered. No books were removed.')