import sqlite3

connection = sqlite3.connect('demo_database.db')

# delete a specific table
connection.execute('DROP TABLE FROM books')

# alter a specific table
connection.execute('ALTER TABLE employees ADD COLUMN transport TEXT')

# writes data to database
connection.commit()

# closes the databases connection
connection.close()