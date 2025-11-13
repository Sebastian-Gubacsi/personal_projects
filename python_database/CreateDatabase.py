"""
create a new database based on user input
"""

import sqlite3
import os


def create_database(db_name):
    """
    Create a new SQLite database in the 'databases/' folder.
    
    Args:
        db_name (str): Name of the database (without .db extension)
    
    Returns:
        sqlite3.Connection: Connection to the newly created database, or None if creation failed
    """
    databases_dir = 'databases'
    
    # Create databases folder if it doesn't exist
    #if not os.path.exists(databases_dir):
        #os.makedirs(databases_dir)
    
    # Create full path to the database file
    db_path = os.path.join(databases_dir, db_name + '.db')
    
    try:
        connection = sqlite3.connect(db_path)
        print(f"Database '{db_name}' created successfully at {db_path}")
        return connection
    except sqlite3.Error as e:
        print(f"Error creating database: {e}")
        return None
