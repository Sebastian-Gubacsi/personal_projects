"""
will support all logic and comunication with the sqlite databases and data manipulation
"""

import sqlite3
import os
import sys

# modul imports
import InsertData as id
import CreateTable as ct
import CollectAndSortData as csd
import RemoveData as rd

# run the program in command interface mod
def cli_mode():
    # variables and initial setup
    databases_dir = 'databases'
    display = True

    # Create databases folder if it doesn't exist
    if not os.path.exists(databases_dir):
        os.makedirs(databases_dir)
        
    # store all database name in the databases folder in a list
    db_files = [f.replace('.db', '') for f in os.listdir(databases_dir) if f.endswith('.db')]

    if not db_files:
        print("No databases found in the databases folder.")
        return
    
    print("Available Databases:")
    for i in range(len(db_files)):
        print(f"{i + 1}: {db_files[i]}")

    pos = int(input("Which database do you want to enter: "))
    db_path = os.path.join(databases_dir, db_files[pos - 1] +'.db')
    connection = sqlite3.connect(db_path)

    # create a table in the database
    ct.create_table(connection)

    # insert data into the table
    id.data_insert(connection)
    
    while display:
        # retrieve and display data
        csd.retrieve_data(connection)

        # remove data from the table based on ID input
        rd.remove_data(connection)
        again = input('Do you want to view and remove more books? (y/n): ')
        if again.lower() != 'y':
            display = False

    connection.close()

def gui_mode():
    """Run the program in graphical user interface mode"""
    from Interface import interface
    interface()

if __name__ == "__main__":
    gui_mode()