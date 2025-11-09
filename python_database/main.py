import sqlite3

connection = sqlite3.connect('demo_database.db')
connection.execute('''
                    CREATE TABLE IF NOT EXISTS books (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        title text,
                        author text,
                        year INTEGER
                    );
''')

# insert data into the table
BooksData = [
    ('The First Programmer', 'Andy Hunt', 1999),
    ('Head First Python', 'Paul Barry', 2010),
    ('Clean Code', 'Robert C. Martin', 2008),
    ('The Pragmatic Programmer', 'Andrew Hunt and David Thomas', 1999),
    ('Introduction to Algorithms', 'Thomas H. Cormen', 2009)
]
connection.execute('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData[0])

# writes data to database
connection.commit()

# closes the databases connection
connection.close()