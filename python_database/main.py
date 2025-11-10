"""
will support all logic and comunication with the sqlite databases and data manipulation
"""

import sqlite3
'''
from Interface import
from InsertData import
from RemoveData import
from CreateTable import
from CreateDatabase import
from RetreiveData import
from SortData import
'''

database_name = input("Enter the database name (with .db extension): ")
connection = sqlite3.connect(database_name)
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
    ('Introduction to Algorithms', 'Thomas H. Cormen', 2009),
]

# connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)

# query data from the table
result = connection.execute('SELECT * FROM books;')
data = result.fetchall()
print(data)

# display queried data
print('Books in Database:')
for row in data:
    print(f'Title: {row[1]}')
    print(f'Author: {row[2]}')
    print(f'Year: {row[3]}')
    print('')

# writes data to database
connection.commit()

# closes the databases connection
connection.close()