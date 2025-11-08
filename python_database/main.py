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

# writes data to database
connection.commit()

# closes the databases connection
connection.close()