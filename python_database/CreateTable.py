"""
create tables in different databases based on user input
"""

def create_table(connection):
    """
    Create a `books` table on the provided sqlite3 `connection` if it doesn't exist.

    Args:
        connection: sqlite3.Connection object
    """
    test = 'books (id INTEGER PRIMARY KEY AUTOINCREMENT, title text, author text, year INTEGER);'
    connection.execute('CREATE TABLE IF NOT EXISTS ' + test)
    # return True to signal success
    return True