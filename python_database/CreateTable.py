"""
Create tables in different databases based on user input
Supports both predefined and custom table creation
"""

def create_table(connection, table_name=None, columns=None):
    """
    Create a table on the provided sqlite3 `connection`.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Optional string for table name. If None, creates default 'books' table
        columns: Optional list of tuples (column_name, column_type, constraints)
                 Example: [('id', 'INTEGER', 'PRIMARY KEY AUTOINCREMENT'),
                          ('name', 'TEXT', 'NOT NULL'),
                          ('age', 'INTEGER', '')]
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        if table_name is None or columns is None:
            # Default books table
            table_def = 'books (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT, year INTEGER);'
            connection.execute('CREATE TABLE IF NOT EXISTS ' + table_def)
            print("Default 'books' table created successfully.")
        else:
            # Custom table
            column_defs = []
            for col in columns:
                col_name, col_type, col_constraints = col
                col_def = f"{col_name} {col_type}"
                if col_constraints:
                    col_def += f" {col_constraints}"
                column_defs.append(col_def)
            
            table_def = f"{table_name} ({', '.join(column_defs)})"
            connection.execute(f'CREATE TABLE IF NOT EXISTS {table_def};')
            print(f"Custom table '{table_name}' created successfully.")
        
        connection.commit()
        return True
    except Exception as e:
        print(f"Error creating table: {e}")
        return False

def get_table_info(connection, table_name):
    """
    Get information about a table's structure.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
    
    Returns:
        list: List of tuples containing column information
    """
    try:
        cursor = connection.cursor()
        cursor.execute(f"PRAGMA table_info({table_name})")
        return cursor.fetchall()
    except Exception as e:
        print(f"Error getting table info: {e}")
        return []

def list_tables(connection):
    """
    List all tables in the database.
    
    Args:
        connection: sqlite3.Connection object
    
    Returns:
        list: List of table names
    """
    try:
        cursor = connection.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
        tables = cursor.fetchall()
        return [table[0] for table in tables]
    except Exception as e:
        print(f"Error listing tables: {e}")
        return []

def drop_table(connection, table_name):
    """
    Drop (delete) a table from the database.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table to drop
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        connection.execute(f"DROP TABLE IF EXISTS {table_name}")
        connection.commit()
        print(f"Table '{table_name}' dropped successfully.")
        return True
    except Exception as e:
        print(f"Error dropping table: {e}")
        return False