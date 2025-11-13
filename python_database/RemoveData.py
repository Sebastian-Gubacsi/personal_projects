'''
This file will handle removing data from the database
'''

def remove_data(connection, book_id=None):
    """
    Remove books from the `books` table based on user input or provided ID.

    Args:
        connection: sqlite3.Connection object
        book_id: Optional integer for GUI mode. If None, will prompt in CLI mode.
    
    Returns:
        bool: True if deletion was successful, False otherwise
    """
    # If book_id not provided, ask user (CLI mode)
    if book_id is None:
        try:
            book_id = int(input('Enter the ID of the book to remove (or 0 to skip): '))
            if book_id <= 0:
                print('No books were removed.')
                return False
        except ValueError:
            print('Invalid ID entered. No books were removed.')
            return False
    
    # Perform the deletion
    try:
        cursor = connection.cursor()
        cursor.execute('DELETE FROM books WHERE id = ?;', (book_id,))
        connection.commit()
        
        if cursor.rowcount > 0:
            print(f'Book with ID {book_id} has been removed.')
            return True
        else:
            print(f'No book found with ID {book_id}.')
            return False
    except Exception as e:
        print(f'Error removing book: {e}')
        return False

def remove_multiple_books(connection, book_ids):
    """
    Remove multiple books from the database at once.
    
    Args:
        connection: sqlite3.Connection object
        book_ids: List of book IDs to remove
    
    Returns:
        int: Number of books successfully removed
    """
    removed_count = 0
    try:
        cursor = connection.cursor()
        for book_id in book_ids:
            cursor.execute('DELETE FROM books WHERE id = ?;', (book_id,))
            if cursor.rowcount > 0:
                removed_count += 1
        connection.commit()
        return removed_count
    except Exception as e:
        print(f'Error removing books: {e}')
        return removed_count