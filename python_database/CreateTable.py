"""
create tables in different databases based on user input
"""

import sqlite3
from main import connection

def create_table():
    test = 'books (id INTEGER PRIMARY KEY AUTOINCREMENT, title text, author text, year INTEGER);'
    connection.execute('CREATE TABLE IF NOT EXISTS ' + test)