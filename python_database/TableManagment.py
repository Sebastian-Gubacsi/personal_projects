"""
this file will mange the table being access currently:
- create cols
- delete cols
- make sure the displayed id updated everytime a item is added or removed (first column is always id 1, second is always id 2, etc)
"""

import sqlite3

def add_column(connection, table_name, column_name, column_type, constraints=""):
    """
    Add a new column to an existing table.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
        column_name: Name of the new column
        column_type: Data type (TEXT, INTEGER, REAL, etc.)
        constraints: Optional constraints (NOT NULL, DEFAULT value, etc.)
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        col_def = f"{column_name} {column_type}"
        if constraints:
            col_def += f" {constraints}"
        
        connection.execute(f"ALTER TABLE {table_name} ADD COLUMN {col_def}")
        connection.commit()
        print(f"Column '{column_name}' added to table '{table_name}' successfully.")
        return True
    except Exception as e:
        print(f"Error adding column: {e}")
        return False

def drop_column(connection, table_name, column_name):
    """
    Drop (remove) a column from a table.
    Note: SQLite has limited ALTER TABLE support. This recreates the table without the column.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
        column_name: Name of the column to remove
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        cursor = connection.cursor()
        
        # Get current table structure
        cursor.execute(f"PRAGMA table_info({table_name})")
        columns = cursor.fetchall()
        
        # Filter out the column to drop
        remaining_columns = [col for col in columns if col[1] != column_name]
        
        if len(remaining_columns) == len(columns):
            print(f"Column '{column_name}' not found in table '{table_name}'.")
            return False
        
        # Build new table definition
        new_columns = []
        for col in remaining_columns:
            col_def = f"{col[1]} {col[2]}"
            if col[3]:  # NOT NULL
                col_def += " NOT NULL"
            if col[4] is not None:  # DEFAULT value
                col_def += f" DEFAULT {col[4]}"
            if col[5]:  # Primary key
                col_def += " PRIMARY KEY"
            new_columns.append(col_def)
        
        # Get column names for data transfer
        col_names = [col[1] for col in remaining_columns]
        col_names_str = ", ".join(col_names)
        
        # Create new table
        temp_table = f"{table_name}_temp"
        cursor.execute(f"CREATE TABLE {temp_table} ({', '.join(new_columns)})")
        
        # Copy data
        cursor.execute(f"INSERT INTO {temp_table} ({col_names_str}) SELECT {col_names_str} FROM {table_name}")
        
        # Drop old table and rename new one
        cursor.execute(f"DROP TABLE {table_name}")
        cursor.execute(f"ALTER TABLE {temp_table} RENAME TO {table_name}")
        
        connection.commit()
        print(f"Column '{column_name}' removed from table '{table_name}' successfully.")
        return True
    except Exception as e:
        print(f"Error dropping column: {e}")
        connection.rollback()
        return False

def reset_ids(connection, table_name, id_column="id"):
    """
    Reset and reorder IDs to be sequential (1, 2, 3, ...).
    This ensures the first row is always ID 1, second is ID 2, etc.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
        id_column: Name of the ID column (default: "id")
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        cursor = connection.cursor()
        
        # Get all data ordered by current ID
        cursor.execute(f"SELECT * FROM {table_name} ORDER BY {id_column}")
        rows = cursor.fetchall()
        
        if not rows:
            print("Table is empty, no IDs to reset.")
            return True
        
        # Get column names
        cursor.execute(f"PRAGMA table_info({table_name})")
        columns = cursor.fetchall()
        column_names = [col[1] for col in columns]
        id_index = column_names.index(id_column)
        
        # Create temporary table with same structure
        temp_table = f"{table_name}_temp_reset"
        
        # Get table creation SQL
        cursor.execute(f"SELECT sql FROM sqlite_master WHERE type='table' AND name='{table_name}'")
        create_sql = cursor.fetchone()[0]
        create_temp_sql = create_sql.replace(f"CREATE TABLE {table_name}", f"CREATE TABLE {temp_table}")
        cursor.execute(create_temp_sql)
        
        # Insert data with new sequential IDs
        non_id_columns = [col for col in column_names if col != id_column]
        non_id_str = ", ".join(non_id_columns)
        
        for new_id, row in enumerate(rows, start=1):
            # Build row data without old ID
            row_data = [val for i, val in enumerate(row) if i != id_index]
            
            placeholders = ", ".join(["?" for _ in range(len(row_data) + 1)])
            cursor.execute(f"INSERT INTO {temp_table} ({id_column}, {non_id_str}) VALUES ({placeholders})", 
                         [new_id] + row_data)
        
        # Drop old table and rename new one
        cursor.execute(f"DROP TABLE {table_name}")
        cursor.execute(f"ALTER TABLE {temp_table} RENAME TO {table_name}")
        
        connection.commit()
        print(f"IDs reset successfully for table '{table_name}'. Now sequential from 1 to {len(rows)}.")
        return True
    except Exception as e:
        print(f"Error resetting IDs: {e}")
        connection.rollback()
        return False

def get_next_id(connection, table_name, id_column="id"):
    """
    Get the next available ID for insertion.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
        id_column: Name of the ID column
    
    Returns:
        int: Next available ID
    """
    try:
        cursor = connection.cursor()
        cursor.execute(f"SELECT MAX({id_column}) FROM {table_name}")
        max_id = cursor.fetchone()[0]
        return (max_id or 0) + 1
    except Exception as e:
        print(f"Error getting next ID: {e}")
        return 1

def rename_column(connection, table_name, old_column_name, new_column_name):
    """
    Rename a column in a table.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
        old_column_name: Current column name
        new_column_name: New column name
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        # SQLite 3.25.0+ supports ALTER TABLE RENAME COLUMN
        connection.execute(f"ALTER TABLE {table_name} RENAME COLUMN {old_column_name} TO {new_column_name}")
        connection.commit()
        print(f"Column '{old_column_name}' renamed to '{new_column_name}' successfully.")
        return True
    except Exception as e:
        print(f"Error renaming column: {e}")
        return False

def get_table_stats(connection, table_name):
    """
    Get statistics about a table.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
    
    Returns:
        dict: Dictionary with table statistics
    """
    try:
        cursor = connection.cursor()
        
        # Get row count
        cursor.execute(f"SELECT COUNT(*) FROM {table_name}")
        row_count = cursor.fetchone()[0]
        
        # Get column count
        cursor.execute(f"PRAGMA table_info({table_name})")
        columns = cursor.fetchall()
        column_count = len(columns)
        
        # Get table size (approximate)
        cursor.execute(f"SELECT SUM(length(quote(*))-length('()'))/1024.0 FROM {table_name}")
        size_kb = cursor.fetchone()[0] or 0
        
        return {
            'table_name': table_name,
            'row_count': row_count,
            'column_count': column_count,
            'size_kb': round(size_kb, 2),
            'columns': [col[1] for col in columns]
        }
    except Exception as e:
        print(f"Error getting table stats: {e}")
        return None

def compact_table(connection, table_name):
    """
    Compact a table by removing fragmentation and optimizing storage.
    This recreates the table with the same structure and data.
    
    Args:
        connection: sqlite3.Connection object
        table_name: Name of the table
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        cursor = connection.cursor()
        
        # Get table structure
        cursor.execute(f"SELECT sql FROM sqlite_master WHERE type='table' AND name='{table_name}'")
        create_sql = cursor.fetchone()[0]
        
        # Create temp table
        temp_table = f"{table_name}_compact_temp"
        create_temp_sql = create_sql.replace(f"CREATE TABLE {table_name}", f"CREATE TABLE {temp_table}")
        cursor.execute(create_temp_sql)
        
        # Copy all data
        cursor.execute(f"INSERT INTO {temp_table} SELECT * FROM {table_name}")
        
        # Drop old and rename
        cursor.execute(f"DROP TABLE {table_name}")
        cursor.execute(f"ALTER TABLE {temp_table} RENAME TO {table_name}")
        
        # Vacuum database
        connection.execute("VACUUM")
        connection.commit()
        
        print(f"Table '{table_name}' compacted successfully.")
        return True
    except Exception as e:
        print(f"Error compacting table: {e}")
        connection.rollback()
        return False