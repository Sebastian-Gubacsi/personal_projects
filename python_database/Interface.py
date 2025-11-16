'''
This file handles the GUI interface for the database
Integrates with existing modules: CreateTable, InsertData, CollectAndSortData, RemoveData, CreateDatabase
'''
import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
import sqlite3
import os

# Import existing modules
import CreateTable as ct
import InsertData as id
import CollectAndSortData as csd
import RemoveData as rd
import CreateDatabase as cd
import TableManagment as tm

class DatabaseInterface:
    def __init__(self, root):
        self.root = root
        self.root.title("Database Manager")
        self.root.geometry("900x700")
        
        self.connection = None
        self.databases_dir = 'databases'
        
        # Create databases folder if it doesn't exist
        if not os.path.exists(self.databases_dir):
            os.makedirs(self.databases_dir)
        
        # Initialize UI first (this creates all widgets including status_label)
        self.setup_ui()
        
    def setup_ui(self):
        # Database Selection Frame
        db_frame = ttk.LabelFrame(self.root, text="Database Selection", padding=10)
        db_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(db_frame, text="Select Database:").grid(row=0, column=0, padx=5, sticky="w")
        
        self.db_combo = ttk.Combobox(db_frame, state="readonly", width=30)
        self.db_combo.grid(row=0, column=1, padx=5)
        # Don't call refresh here - it will be called after UI setup is complete
        
        ttk.Button(db_frame, text="Connect", command=self.connect_database).grid(row=0, column=2, padx=5)
        ttk.Button(db_frame, text="Refresh List", command=self.refresh_database_list).grid(row=0, column=3, padx=5)
        ttk.Button(db_frame, text="Create New DB", command=self.create_new_database).grid(row=0, column=4, padx=5)
        
        # Table Selection (shown after database is connected)
        ttk.Label(db_frame, text="Current Table:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        
        self.table_combo = ttk.Combobox(db_frame, state="readonly", width=30)
        self.table_combo.grid(row=1, column=1, padx=5, pady=5)
        self.table_combo.bind('<<ComboboxSelected>>', lambda e: self.load_data())
        
        ttk.Button(db_frame, text="Refresh Tables", command=self.refresh_table_list).grid(row=1, column=2, padx=5, pady=5)
        ttk.Button(db_frame, text="View Table Info", command=self.show_table_info).grid(row=1, column=3, padx=5, pady=5)
        
        # Data Display Frame
        display_frame = ttk.LabelFrame(self.root, text="Data Viewer", padding=10)
        display_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        # Treeview for displaying data (will be configured dynamically)
        self.tree = ttk.Treeview(display_frame, show="headings", selectmode="browse")
        self.tree.pack(side="left", fill="both", expand=True)
        
        # Scrollbar
        scrollbar = ttk.Scrollbar(display_frame, orient="vertical", command=self.tree.yview)
        scrollbar.pack(side="right", fill="y")
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        # Action Buttons Frame
        action_frame = ttk.LabelFrame(self.root, text="Actions", padding=10)
        action_frame.pack(fill="x", padx=10, pady=5)
        
        # Row 1: Data operations
        data_frame = ttk.Frame(action_frame)
        data_frame.pack(fill="x", pady=2)
        
        ttk.Button(data_frame, text="🔄 Refresh Data", command=self.load_data).pack(side="left", padx=5)
        ttk.Button(data_frame, text="➕ Add Records", command=self.insert_data).pack(side="left", padx=5)
        ttk.Button(data_frame, text="✏️ Add Single Record", command=self.add_single_book).pack(side="left", padx=5)
        ttk.Button(data_frame, text="🗑️ Delete Selected", command=self.delete_selected).pack(side="left", padx=5)
        
        # Row 2: Table operations
        table_frame = ttk.Frame(action_frame)
        table_frame.pack(fill="x", pady=2)
        
        ttk.Button(table_frame, text="📋 Create Table", command=self.create_table).pack(side="left", padx=5)
        ttk.Button(table_frame, text="➕ Add Column", command=self.add_column_dialog).pack(side="left", padx=5)
        ttk.Button(table_frame, text="➖ Remove Column", command=self.remove_column_dialog).pack(side="left", padx=5)
        ttk.Button(table_frame, text="🔢 Reset IDs", command=self.reset_table_ids).pack(side="left", padx=5)
        ttk.Button(table_frame, text="📊 Table Stats", command=self.show_table_stats).pack(side="left", padx=5)
        
        # Status Bar
        self.status_label = ttk.Label(self.root, text="Not connected to any database", relief="sunken", anchor="w")
        self.status_label.pack(fill="x", side="bottom", padx=5, pady=2)
        
        # Now that all UI is set up, refresh the database list
        self.refresh_database_list()
        
    def refresh_database_list(self):
        """Refresh the list of available databases"""
        try:
            db_files = [f.replace('.db', '') for f in os.listdir(self.databases_dir) 
                        if f.endswith('.db')]
            self.db_combo['values'] = db_files
            
            # Don't auto-select a database - let user choose
            if db_files:
                self.status_label.config(text=f"Found {len(db_files)} database(s) - Please select and connect")
            else:
                self.status_label.config(text="No databases found - Create a new database to get started")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to list databases: {str(e)}")
    
    def create_new_database(self):
        """Create a new database using CreateDatabase module"""
        db_name = simpledialog.askstring("Create Database", "Enter new database name (without .db):")
        
        if db_name:
            # Clean the name
            db_name = db_name.strip()
            if db_name:
                try:
                    # Use the CreateDatabase module
                    connection = cd.create_database(db_name)
                    if connection:
                        connection.close()
                        self.refresh_database_list()
                        # Select the newly created database
                        self.db_combo.set(db_name)
                        messagebox.showinfo("Success", f"Database '{db_name}' created successfully!")
                except Exception as e:
                    messagebox.showerror("Error", f"Failed to create database: {str(e)}")
    
    def connect_database(self):
        """Connect to the selected database"""
        if not self.db_combo.get():
            messagebox.showwarning("Warning", "Please select a database")
            return
        
        try:
            if self.connection:
                self.connection.close()
            
            db_path = os.path.join(self.databases_dir, self.db_combo.get() + '.db')
            self.connection = sqlite3.connect(db_path)
            self.status_label.config(text=f"Connected to: {self.db_combo.get()}.db")
            self.refresh_table_list()
            messagebox.showinfo("Success", f"Connected to {self.db_combo.get()}.db")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to connect: {str(e)}")
    
    def create_table(self):
        """Create books table or custom table using CreateTable module"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        # Ask user which type of table to create
        dialog = tk.Toplevel(self.root)
        dialog.title("Create Table")
        dialog.geometry("300x150")
        dialog.transient(self.root)
        dialog.grab_set()
        
        ttk.Label(dialog, text="What type of table do you want to create?", 
                 font=("Arial", 10)).pack(pady=20)
        
        button_frame = ttk.Frame(dialog)
        button_frame.pack(pady=10)
        
        def create_default():
            dialog.destroy()
            try:
                result = ct.create_table(self.connection)
                if result:
                    messagebox.showinfo("Success", "Books table created/verified successfully!")
                    self.load_data()
            except Exception as e:
                messagebox.showerror("Error", f"Failed to create table: {str(e)}")
        
        def create_custom():
            dialog.destroy()
            self.create_custom_table_dialog()
        
        ttk.Button(button_frame, text="Default Books Table", 
                  command=create_default, width=20).pack(pady=5)
        ttk.Button(button_frame, text="Custom Table", 
                  command=create_custom, width=20).pack(pady=5)
        ttk.Button(button_frame, text="Cancel", 
                  command=dialog.destroy, width=20).pack(pady=5)
    
    def load_data(self):
        """Load and display data from the selected table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            self.status_label.config(text="No table selected")
            return
        
        try:
            # Clear existing data
            self.tree.delete(*self.tree.get_children())
            
            cursor = self.connection.cursor()
            
            # Get column names dynamically
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns_info = cursor.fetchall()
            columns = [col[1] for col in columns_info]
            
            # Configure treeview columns
            self.tree['columns'] = columns
            
            for col in columns:
                self.tree.heading(col, text=col.capitalize())
                # Adjust width based on column name length
                self.tree.column(col, width=max(100, len(col) * 10))
            
            # Fetch and display data
            cursor.execute(f"SELECT * FROM {table_name}")
            rows = cursor.fetchall()
            
            for row in rows:
                self.tree.insert("", "end", values=row)
            
            self.status_label.config(text=f"Loaded {len(rows)} record(s) from '{table_name}' table")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load data: {str(e)}")
    
    def insert_data(self):
        """Insert multiple placeholder records into the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            # Get table structure
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns_info = cursor.fetchall()
            
            # Filter out auto-increment columns
            editable_columns = [col for col in columns_info 
                              if 'AUTOINCREMENT' not in str(col).upper() and col[5] == 0]
            
            if not editable_columns:
                messagebox.showwarning("Warning", "No editable columns found in this table")
                return
            
            # Ask for number of records to add
            num_records = simpledialog.askinteger(
                "Insert Records", 
                f"How many placeholder records do you want to add to '{table_name}'?",
                minvalue=1,
                maxvalue=100
            )
            
            if num_records is None:
                return
            
            # Create placeholder data based on column types
            records = []
            for i in range(num_records):
                record = []
                for col in editable_columns:
                    col_name = col[1]
                    col_type = col[2].upper()
                    
                    # Generate placeholder based on type
                    if 'INT' in col_type:
                        record.append(0)
                    elif 'REAL' in col_type or 'FLOAT' in col_type or 'DOUBLE' in col_type:
                        record.append(0.0)
                    else:  # TEXT, BLOB, etc.
                        record.append(f'placeholder_{col_name}')
                
                records.append(tuple(record))
            
            # Build INSERT query
            column_names = [col[1] for col in editable_columns]
            columns_str = ', '.join(column_names)
            placeholders = ', '.join(['?' for _ in column_names])
            query = f"INSERT INTO {table_name} ({columns_str}) VALUES ({placeholders})"
            
            # Insert records
            self.connection.executemany(query, records)
            self.connection.commit()
            
            messagebox.showinfo("Success", f"Inserted {num_records} placeholder record(s) into '{table_name}'")
            self.load_data()
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to insert data: {str(e)}")
    
    def add_single_book(self):
        """Add a single record to the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            # Get table structure
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns_info = cursor.fetchall()
            
            # Filter out auto-increment columns
            editable_columns = [col for col in columns_info 
                              if 'AUTOINCREMENT' not in col[2].upper() and col[5] == 0]
            
            if not editable_columns:
                messagebox.showinfo("Info", "No editable columns found in this table")
                return
            
            # Create a dialog window
            dialog = tk.Toplevel(self.root)
            dialog.title(f"Add Record to {table_name}")
            dialog.geometry("450x400")
            dialog.transient(self.root)
            dialog.grab_set()
            
            # Create scrollable frame
            canvas = tk.Canvas(dialog)
            scrollbar = ttk.Scrollbar(dialog, orient="vertical", command=canvas.yview)
            scrollable_frame = ttk.Frame(canvas)
            
            scrollable_frame.bind(
                "<Configure>",
                lambda e: canvas.configure(scrollregion=canvas.bbox("all"))
            )
            
            canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
            canvas.configure(yscrollcommand=scrollbar.set)
            
            # Create input fields for each column
            entries = {}
            for i, col in enumerate(editable_columns):
                col_id, col_name, col_type, not_null, default_val, pk = col
                
                ttk.Label(scrollable_frame, text=f"{col_name}:").grid(
                    row=i, column=0, padx=10, pady=5, sticky="w")
                
                entry = ttk.Entry(scrollable_frame, width=30)
                entry.grid(row=i, column=1, padx=10, pady=5)
                
                # Show type and constraints
                constraint_text = f"({col_type}"
                if not_null:
                    constraint_text += ", NOT NULL"
                if default_val:
                    constraint_text += f", default: {default_val}"
                constraint_text += ")"
                
                ttk.Label(scrollable_frame, text=constraint_text, 
                         font=("Arial", 8), foreground="gray").grid(
                    row=i, column=2, padx=5, pady=5, sticky="w")
                
                entries[col_name] = entry
            
            canvas.pack(side="left", fill="both", expand=True, padx=10, pady=10)
            scrollbar.pack(side="right", fill="y")
            
            def save_record():
                # Collect values
                values = []
                column_names = []
                
                for col_name, entry in entries.items():
                    value = entry.get().strip()
                    if value:  # Only include if value provided
                        column_names.append(col_name)
                        values.append(value)
                
                if not values:
                    messagebox.showwarning("Warning", "Please fill in at least one field")
                    return
                
                try:
                    placeholders = ', '.join(['?' for _ in values])
                    columns_str = ', '.join(column_names)
                    query = f"INSERT INTO {table_name} ({columns_str}) VALUES ({placeholders})"
                    
                    self.connection.execute(query, values)
                    self.connection.commit()
                    messagebox.showinfo("Success", "Record added successfully!")
                    dialog.destroy()
                    self.load_data()
                except Exception as e:
                    messagebox.showerror("Error", f"Failed to add record: {str(e)}")
            
            # Buttons
            button_frame = ttk.Frame(dialog)
            button_frame.pack(side="bottom", pady=10)
            
            ttk.Button(button_frame, text="Save", command=save_record).pack(side="left", padx=5)
            ttk.Button(button_frame, text="Cancel", command=dialog.destroy).pack(side="left", padx=5)
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to open add record dialog: {str(e)}")
    
    def delete_selected(self):
        """Delete the selected record from the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("Warning", "Please select a record to delete")
            return
        
        try:
            cursor = self.connection.cursor()
            
            # Get table structure to find primary key
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns_info = cursor.fetchall()
            
            # Find primary key column (usually the first column or marked as PK)
            pk_column = None
            for col in columns_info:
                if col[5] == 1:  # col[5] is the pk flag
                    pk_column = col[1]  # col[1] is the column name
                    break
            
            if not pk_column:
                # If no PK, use first column
                pk_column = columns_info[0][1]
            
            # Get the selected record's data
            item = self.tree.item(selected[0])
            pk_value = item['values'][0]  # First value is usually the ID/PK
            
            # Confirm deletion
            confirm = messagebox.askyesno(
                "Confirm Deletion", 
                f"Delete record from '{table_name}'?\n\n{pk_column}: {pk_value}\n\nAre you sure?"
            )
            
            if confirm:
                cursor.execute(f"DELETE FROM {table_name} WHERE {pk_column} = ?", (pk_value,))
                self.connection.commit()
                
                messagebox.showinfo("Success", f"Record deleted successfully")
                self.load_data()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to delete record: {str(e)}")
    
    def create_custom_table_dialog(self):
        """Open dialog to create a custom table"""
        dialog = tk.Toplevel(self.root)
        dialog.title("Create Custom Table")
        dialog.geometry("600x500")
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Table name
        ttk.Label(dialog, text="Table Name:", font=("Arial", 10, "bold")).grid(
            row=0, column=0, padx=10, pady=10, sticky="w")
        table_name_entry = ttk.Entry(dialog, width=30)
        table_name_entry.grid(row=0, column=1, padx=10, pady=10, columnspan=2)
        
        # Column list frame
        columns_frame = ttk.LabelFrame(dialog, text="Columns", padding=10)
        columns_frame.grid(row=1, column=0, columnspan=3, padx=10, pady=10, sticky="nsew")
        
        # Headers
        ttk.Label(columns_frame, text="Column Name", font=("Arial", 9, "bold")).grid(
            row=0, column=0, padx=5, pady=5)
        ttk.Label(columns_frame, text="Data Type", font=("Arial", 9, "bold")).grid(
            row=0, column=1, padx=5, pady=5)
        ttk.Label(columns_frame, text="Constraints", font=("Arial", 9, "bold")).grid(
            row=0, column=2, padx=5, pady=5)
        
        # Store column entry widgets
        column_entries = []
        
        def add_column_row(row_num, col_name="", col_type="TEXT", col_constraint=""):
            """Add a row of column input fields"""
            name_entry = ttk.Entry(columns_frame, width=20)
            name_entry.grid(row=row_num, column=0, padx=5, pady=5)
            name_entry.insert(0, col_name)
            
            type_combo = ttk.Combobox(columns_frame, width=15, 
                                     values=["TEXT", "INTEGER", "REAL", "BLOB", "NUMERIC"],
                                     state="readonly")
            type_combo.grid(row=row_num, column=1, padx=5, pady=5)
            type_combo.set(col_type)
            
            constraint_entry = ttk.Entry(columns_frame, width=25)
            constraint_entry.grid(row=row_num, column=2, padx=5, pady=5)
            constraint_entry.insert(0, col_constraint)
            
            # Delete button
            delete_btn = ttk.Button(columns_frame, text="X", width=3,
                                   command=lambda: remove_column_row(row_num))
            delete_btn.grid(row=row_num, column=3, padx=5, pady=5)
            
            column_entries.append({
                'row': row_num,
                'name': name_entry,
                'type': type_combo,
                'constraint': constraint_entry,
                'delete_btn': delete_btn
            })
        
        def remove_column_row(row_num):
            """Remove a column row"""
            for i, entry in enumerate(column_entries):
                if entry['row'] == row_num:
                    entry['name'].destroy()
                    entry['type'].destroy()
                    entry['constraint'].destroy()
                    entry['delete_btn'].destroy()
                    column_entries.pop(i)
                    break
        
        # Add initial rows with ID column
        add_column_row(1, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT")
        add_column_row(2, "name", "TEXT", "NOT NULL")
        add_column_row(3, "created_at", "TEXT", "DEFAULT CURRENT_TIMESTAMP")
        
        # Add column button
        add_btn = ttk.Button(dialog, text="+ Add Column", 
        command=lambda: add_column_row(len(column_entries) + 1))
        add_btn.grid(row=2, column=0, columnspan=3, pady=10)
        
        # Info label
        info_text = "Common constraints: PRIMARY KEY, NOT NULL, UNIQUE, DEFAULT value, CHECK(...)"
        ttk.Label(dialog, text=info_text, font=("Arial", 8), foreground="gray").grid(
            row=3, column=0, columnspan=3, padx=10, pady=5)
        
        def save_table():
            """Save the custom table"""
            table_name = table_name_entry.get().strip()
            
            if not table_name:
                messagebox.showwarning("Warning", "Please enter a table name")
                return
            
            # Collect column definitions
            columns = []
            for entry in column_entries:
                col_name = entry['name'].get().strip()
                col_type = entry['type'].get().strip()
                col_constraint = entry['constraint'].get().strip()
                
                if col_name:  # Only add if column name is provided
                    columns.append((col_name, col_type, col_constraint))
            
            if not columns:
                messagebox.showwarning("Warning", "Please add at least one column")
                return
            
            try:
                # Use the CreateTable module to create custom table
                result = ct.create_table(self.connection, table_name, columns)
                if result:
                    messagebox.showinfo("Success", f"Table '{table_name}' created successfully!")
                    dialog.destroy()
                    self.refresh_table_list()
            except Exception as e:
                messagebox.showerror("Error", f"Failed to create table: {str(e)}")
        
        # Action buttons
        button_frame = ttk.Frame(dialog)
        button_frame.grid(row=4, column=0, columnspan=3, pady=20)
        
        ttk.Button(button_frame, text="Create Table", command=save_table).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Cancel", command=dialog.destroy).pack(side="left", padx=5)
        
        dialog.grid_rowconfigure(1, weight=1)
        dialog.grid_columnconfigure(1, weight=1)
    
    def refresh_table_list(self):
        """Refresh the list of tables in the current database"""
        if not self.connection:
            return
        
        try:
            tables = ct.list_tables(self.connection)
            self.table_combo['values'] = tables
            if tables:
                self.table_combo.current(0)
                self.load_data()
            else:
                self.status_label.config(text="No tables found in database")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to list tables: {str(e)}")
    
    def show_table_info(self):
        """Show detailed information about the selected table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            info = ct.get_table_info(self.connection, table_name)
            
            # Create info dialog
            dialog = tk.Toplevel(self.root)
            dialog.title(f"Table Info: {table_name}")
            dialog.geometry("600x400")
            dialog.transient(self.root)
            
            # Create treeview for info
            info_tree = ttk.Treeview(dialog, columns=("ID", "Name", "Type", "NotNull", "DefaultVal", "PK"), 
                                    show="headings")
            
            info_tree.heading("ID", text="Column ID")
            info_tree.heading("Name", text="Name")
            info_tree.heading("Type", text="Type")
            info_tree.heading("NotNull", text="Not Null")
            info_tree.heading("DefaultVal", text="Default Value")
            info_tree.heading("PK", text="Primary Key")
            
            for col in info:
                info_tree.insert("", "end", values=col)
            
            info_tree.pack(fill="both", expand=True, padx=10, pady=10)
            
            ttk.Button(dialog, text="Close", command=dialog.destroy).pack(pady=10)
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to get table info: {str(e)}")
    
    def add_column_dialog(self):
        """Open dialog to add a new column to the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        # Create dialog
        dialog = tk.Toplevel(self.root)
        dialog.title(f"Add Column to {table_name}")
        dialog.geometry("450x250")
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Column name
        ttk.Label(dialog, text="Column Name:").grid(row=0, column=0, padx=10, pady=10, sticky="w")
        name_entry = ttk.Entry(dialog, width=30)
        name_entry.grid(row=0, column=1, padx=10, pady=10)
        
        # Data type
        ttk.Label(dialog, text="Data Type:").grid(row=1, column=0, padx=10, pady=10, sticky="w")
        type_combo = ttk.Combobox(dialog, width=28, 
                                  values=["TEXT", "INTEGER", "REAL", "BLOB", "NUMERIC"],
                                  state="readonly")
        type_combo.grid(row=1, column=1, padx=10, pady=10)
        type_combo.set("TEXT")
        
        # Constraints
        ttk.Label(dialog, text="Constraints:").grid(row=2, column=0, padx=10, pady=10, sticky="w")
        constraint_entry = ttk.Entry(dialog, width=30)
        constraint_entry.grid(row=2, column=1, padx=10, pady=10)
        
        ttk.Label(dialog, text="(e.g., NOT NULL, DEFAULT 'value')", 
                 font=("Arial", 8), foreground="gray").grid(row=3, column=1, padx=10, sticky="w")
        
        def add_column():
            col_name = name_entry.get().strip()
            col_type = type_combo.get()
            constraints = constraint_entry.get().strip()
            
            if not col_name:
                messagebox.showwarning("Warning", "Please enter a column name")
                return
            
            try:
                result = tm.add_column(self.connection, table_name, col_name, col_type, constraints)
                if result:
                    messagebox.showinfo("Success", f"Column '{col_name}' added successfully!")
                    dialog.destroy()
                    self.load_data()
            except Exception as e:
                messagebox.showerror("Error", f"Failed to add column: {str(e)}")
        
        # Buttons
        button_frame = ttk.Frame(dialog)
        button_frame.grid(row=4, column=0, columnspan=2, pady=20)
        
        ttk.Button(button_frame, text="Add Column", command=add_column).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Cancel", command=dialog.destroy).pack(side="left", padx=5)
    
    def remove_column_dialog(self):
        """Open dialog to remove a column from the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            # Get current columns
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns = cursor.fetchall()
            column_names = [col[1] for col in columns]
            
            if len(column_names) <= 1:
                messagebox.showwarning("Warning", "Cannot remove column. Table must have at least one column.")
                return
            
            # Create dialog
            dialog = tk.Toplevel(self.root)
            dialog.title(f"Remove Column from {table_name}")
            dialog.geometry("400x200")
            dialog.transient(self.root)
            dialog.grab_set()
            
            ttk.Label(dialog, text="Select column to remove:", 
                     font=("Arial", 10, "bold")).pack(pady=20)
            
            # Column selector
            col_combo = ttk.Combobox(dialog, width=30, values=column_names, state="readonly")
            col_combo.pack(pady=10)
            if column_names:
                col_combo.current(0)
            
            ttk.Label(dialog, text="⚠️ Warning: This action cannot be undone!", 
                     font=("Arial", 9), foreground="red").pack(pady=10)
            
            def remove_column():
                col_name = col_combo.get()
                if not col_name:
                    messagebox.showwarning("Warning", "Please select a column")
                    return
                
                confirm = messagebox.askyesno(
                    "Confirm Deletion",
                    f"Are you sure you want to remove column '{col_name}'?\n\nThis will delete all data in this column!"
                )
                
                if confirm:
                    try:
                        result = tm.drop_column(self.connection, table_name, col_name)
                        if result:
                            messagebox.showinfo("Success", f"Column '{col_name}' removed successfully!")
                            dialog.destroy()
                            self.load_data()
                    except Exception as e:
                        messagebox.showerror("Error", f"Failed to remove column: {str(e)}")
            
            # Buttons
            button_frame = ttk.Frame(dialog)
            button_frame.pack(pady=20)
            
            ttk.Button(button_frame, text="Remove Column", command=remove_column).pack(side="left", padx=5)
            ttk.Button(button_frame, text="Cancel", command=dialog.destroy).pack(side="left", padx=5)
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to open remove column dialog: {str(e)}")
    
    def reset_table_ids(self):
        """Reset IDs in the current table to be sequential (1, 2, 3, ...)"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            # Get table structure to find ID column
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns = cursor.fetchall()
            
            # Find primary key or first column
            id_column = None
            for col in columns:
                if col[5] == 1:  # Primary key
                    id_column = col[1]
                    break
            
            if not id_column:
                id_column = columns[0][1]  # Use first column
            
            confirm = messagebox.askyesno(
                "Confirm ID Reset",
                f"Reset IDs in column '{id_column}' to be sequential (1, 2, 3, ...)?\n\n"
                f"This will reorder all IDs in the '{table_name}' table."
            )
            
            if confirm:
                result = tm.reset_ids(self.connection, table_name, id_column)
                if result:
                    messagebox.showinfo("Success", "IDs reset successfully! All IDs are now sequential.")
                    self.load_data()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to reset IDs: {str(e)}")
    
    def show_table_stats(self):
        """Show statistics about the current table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        table_name = self.table_combo.get()
        if not table_name:
            messagebox.showwarning("Warning", "Please select a table")
            return
        
        try:
            stats = tm.get_table_stats(self.connection, table_name)
            
            if stats:
                # Create stats dialog
                dialog = tk.Toplevel(self.root)
                dialog.title(f"Table Statistics: {table_name}")
                dialog.geometry("400x300")
                dialog.transient(self.root)
                
                # Display stats
                info_frame = ttk.Frame(dialog, padding=20)
                info_frame.pack(fill="both", expand=True)
                
                ttk.Label(info_frame, text=f"Table: {stats['table_name']}", 
                         font=("Arial", 12, "bold")).pack(pady=10)
                
                ttk.Label(info_frame, text=f"Total Rows: {stats['row_count']}", 
                         font=("Arial", 10)).pack(pady=5)
                
                ttk.Label(info_frame, text=f"Total Columns: {stats['column_count']}", 
                         font=("Arial", 10)).pack(pady=5)
                
                ttk.Label(info_frame, text=f"Approximate Size: {stats['size_kb']} KB", 
                         font=("Arial", 10)).pack(pady=5)
                
                ttk.Separator(info_frame, orient="horizontal").pack(fill="x", pady=10)
                
                ttk.Label(info_frame, text="Columns:", 
                         font=("Arial", 10, "bold")).pack(pady=5)
                
                col_text = ", ".join(stats['columns'])
                ttk.Label(info_frame, text=col_text, 
                         font=("Arial", 9), wraplength=350).pack(pady=5)
                
                ttk.Button(dialog, text="Close", command=dialog.destroy).pack(pady=10)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to get table statistics: {str(e)}")
    
    def on_closing(self):
        """Handle window close event"""
        if self.connection:
            self.connection.close()
        self.root.destroy()

def interface():
    """
    Launch the database interface
    """
    root = tk.Tk()
    app = DatabaseInterface(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":
    interface()