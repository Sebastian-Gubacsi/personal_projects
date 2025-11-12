"""
create tables in different databases based on user input
"""

import sqlite3
import main as m

def create_table():
    test = 'books (id INTEGER PRIMARY KEY AUTOINCREMENT, title text, author text, year INTEGER);'
    m.connection.execute('CREATE TABLE IF NOT EXISTS ' + test)