'''
this file handles the GUI interface for the database
'''
import tkinter as tk
from tkinter import ttk, messagebox
import sqlite3
import os

class DatabaseInterface:
    def __init__(self, root):
        self.root = root
        self.root.title("Database Manager")
        self.root.geometry("800x600")
        
        self.connection = None
        self.databases_dir = 'databases'
        
        # Create databases folder if it doesn't exist
        if not os.path.exists(self.databases_dir):
            os.makedirs(self.databases_dir)
        
        self.setup_ui()
        
    def setup_ui(self):
        # Database Selection Frame
        db_frame = ttk.LabelFrame(self.root, text="Database Selection", padding=10)
        db_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(db_frame, text="Select Database:").grid(row=0, column=0, padx=5)
        
        self.db_combo = ttk.Combobox(db_frame, state="readonly", width=30)
        self.db_combo.grid(row=0, column=1, padx=5)
        self.refresh_database_list()
        
        ttk.Button(db_frame, text="Connect", command=self.connect_database).grid(row=0, column=2, padx=5)
        ttk.Button(db_frame, text="Refresh", command=self.refresh_database_list).grid(row=0, column=3, padx=5)
        
        # Data Display Frame
        display_frame = ttk.LabelFrame(self.root, text="Data Display", padding=10)
        display_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        # Treeview for displaying data
        self.tree = ttk.Treeview(display_frame, show="headings")
        self.tree.pack(side="left", fill="both", expand=True)
        
        scrollbar = ttk.Scrollbar(display_frame, orient="vertical", command=self.tree.yview)
        scrollbar.pack(side="right", fill="y")
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        # Action Buttons Frame
        action_frame = ttk.Frame(self.root, padding=10)
        action_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Button(action_frame, text="Refresh Data", command=self.load_data).pack(side="left", padx=5)
        ttk.Button(action_frame, text="Add Record", command=self.add_record_window).pack(side="left", padx=5)
        ttk.Button(action_frame, text="Delete Selected", command=self.delete_record).pack(side="left", padx=5)
        ttk.Button(action_frame, text="Create Table", command=self.create_table_window).pack(side="left", padx=5)
        
        # Status Bar
        self.status_label = ttk.Label(self.root, text="Not connected to any database", relief="sunken")
        self.status_label.pack(fill="x", side="bottom")
        
    def refresh_database_list(self):
        """Refresh the list of available databases"""
        db_files = [f.replace('.db', '') for f in os.listdir(self.databases_dir) 
                    if f.endswith('.db')]
        self.db_combo['values'] = db_files
        if db_files:
            self.db_combo.current(0)
    
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
            self.load_data()
            messagebox.showinfo("Success", f"Connected to {self.db_combo.get()}.db")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to connect: {str(e)}")
    
    def load_data(self):
        """Load and display data from the database"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        try:
            cursor = self.connection.cursor()
            
            # Get table names
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
            tables = cursor.fetchall()
            
            if not tables:
                messagebox.showinfo("Info", "No tables found in database")
                return
            
            # For simplicity, use the first table
            table_name = tables[0][0]
            
            # Get column names
            cursor.execute(f"PRAGMA table_info({table_name})")
            columns = [col[1] for col in cursor.fetchall()]
            
            # Configure treeview columns
            self.tree.delete(*self.tree.get_children())
            self.tree['columns'] = columns
            
            for col in columns:
                self.tree.heading(col, text=col)
                self.tree.column(col, width=100)
            
            # Fetch and display data
            cursor.execute(f"SELECT * FROM {table_name}")
            rows = cursor.fetchall()
            
            for row in rows:
                self.tree.insert("", "end", values=row)
            
            self.status_label.config(text=f"Loaded {len(rows)} records from {table_name}")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load data: {str(e)}")
    
    def delete_record(self):
        """Delete the selected record"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("Warning", "Please select a record to delete")
            return
        
        try:
            cursor = self.connection.cursor()
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
            table_name = cursor.fetchall()[0][0]
            
            # Get the ID from the first column of selected row
            item = self.tree.item(selected[0])
            record_id = item['values'][0]
            
            # Get the first column name (assuming it's the ID)
            cursor.execute(f"PRAGMA table_info({table_name})")
            id_column = cursor.fetchall()[0][1]
            
            confirm = messagebox.askyesno("Confirm", f"Delete record with {id_column}={record_id}?")
            if confirm:
                cursor.execute(f"DELETE FROM {table_name} WHERE {id_column}=?", (record_id,))
                self.connection.commit()
                self.load_data()
                messagebox.showinfo("Success", "Record deleted successfully")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to delete record: {str(e)}")
    
    def add_record_window(self):
        """Open a window to add a new record"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        # This would open a new window with input fields
        # Implementation depends on your table structure
        messagebox.showinfo("Info", "Add record functionality - implement based on your table structure")
    
    def create_table_window(self):
        """Open a window to create a new table"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        messagebox.showinfo("Info", "Create table functionality - implement based on your needs")
    
    def __del__(self):
        """Close database connection when interface is destroyed"""
        if self.connection:
            self.connection.close()

def interface():
    """
    Launch the database interface
    """
    root = tk.Tk()
    app = DatabaseInterface(root)
    root.mainloop()

if __name__ == "__main__":
    interface()