import sqlite3

connection = sqlite3.connect('demo_database.db')

# delete a specific book by ID from the books table
book_ID_to_delete = 1
connection.execute('DELETE FROM books WHERE id = ?;', (book_ID_to_delete,))

# writes data to database
connection.commit()

result = connection.execute('SELECT * FROM books;')
print('books')
for row in result.fetchall():
    print(f'Title: {row[1]}')
    print(f'Author: {row[2]}')
    print(f'Year: {row[3]}')
    print('')

# closes the databases connection
connection.close()