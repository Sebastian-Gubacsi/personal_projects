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
        
        self.setup_ui()
        
    def setup_ui(self):
        # Database Selection Frame
        db_frame = ttk.LabelFrame(self.root, text="Database Selection", padding=10)
        db_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(db_frame, text="Select Database:").grid(row=0, column=0, padx=5, sticky="w")
        
        self.db_combo = ttk.Combobox(db_frame, state="readonly", width=30)
        self.db_combo.grid(row=0, column=1, padx=5)
        self.refresh_database_list()
        
        ttk.Button(db_frame, text="Connect", command=self.connect_database).grid(row=0, column=2, padx=5)
        ttk.Button(db_frame, text="Refresh List", command=self.refresh_database_list).grid(row=0, column=3, padx=5)
        ttk.Button(db_frame, text="Create New DB", command=self.create_new_database).grid(row=0, column=4, padx=5)
        
        # Data Display Frame
        display_frame = ttk.LabelFrame(self.root, text="Books in Database", padding=10)
        display_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        # Treeview for displaying data
        columns = ("ID", "Title", "Author", "Year")
        self.tree = ttk.Treeview(display_frame, columns=columns, show="headings", selectmode="browse")
        
        # Define column headings
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=150)
        
        self.tree.column("ID", width=50)
        
        self.tree.pack(side="left", fill="both", expand=True)
        
        # Scrollbar
        scrollbar = ttk.Scrollbar(display_frame, orient="vertical", command=self.tree.yview)
        scrollbar.pack(side="right", fill="y")
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        # Action Buttons Frame
        action_frame = ttk.LabelFrame(self.root, text="Actions", padding=10)
        action_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Button(action_frame, text="🔄 Refresh Data", command=self.load_data).pack(side="left", padx=5)
        ttk.Button(action_frame, text="📋 Create Table", command=self.create_table).pack(side="left", padx=5)
        ttk.Button(action_frame, text="➕ Add Records", command=self.insert_data).pack(side="left", padx=5)
        ttk.Button(action_frame, text="✏️ Add Single Book", command=self.add_single_book).pack(side="left", padx=5)
        ttk.Button(action_frame, text="🗑️ Delete Selected", command=self.delete_selected).pack(side="left", padx=5)
        
        # Status Bar
        self.status_label = ttk.Label(self.root, text="Not connected to any database", relief="sunken", anchor="w")
        self.status_label.pack(fill="x", side="bottom", padx=5, pady=2)
        
    def refresh_database_list(self):
        """Refresh the list of available databases"""
        try:
            db_files = [f.replace('.db', '') for f in os.listdir(self.databases_dir) 
                        if f.endswith('.db')]
            self.db_combo['values'] = db_files
            if db_files:
                self.db_combo.current(0)
            self.status_label.config(text=f"Found {len(db_files)} database(s)")
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
            self.load_data()
            messagebox.showinfo("Success", f"Connected to {self.db_combo.get()}.db")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to connect: {str(e)}")
    
    def create_table(self):
        """Create books table using CreateTable module"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        try:
            # Use the CreateTable module
            result = ct.create_table(self.connection)
            if result:
                messagebox.showinfo("Success", "Books table created/verified successfully!")
                self.load_data()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to create table: {str(e)}")
    
    def load_data(self):
        """Load and display data using CollectAndSortData module"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        try:
            # Clear existing data
            self.tree.delete(*self.tree.get_children())
            
            # Check if books table exists
            cursor = self.connection.cursor()
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='books'")
            if not cursor.fetchone():
                self.status_label.config(text="No 'books' table found. Please create table first.")
                return
            
            # Use the CollectAndSortData module to retrieve data
            data = csd.retrieve_data(self.connection)
            
            # Display data in treeview
            for row in data:
                self.tree.insert("", "end", values=row)
            
            self.status_label.config(text=f"Loaded {len(data)} record(s) from books table")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load data: {str(e)}")
    
    def insert_data(self):
        """Insert data using InsertData module"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        try:
            # Check if table exists first
            cursor = self.connection.cursor()
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='books'")
            if not cursor.fetchone():
                messagebox.showwarning("Warning", "Please create the 'books' table first")
                return
            
            # Ask for number of records to add
            num_records = simpledialog.askinteger(
                "Insert Records", 
                "How many placeholder records do you want to add?",
                minvalue=0,
                maxvalue=100
            )
            
            if num_records is not None:
                # Temporarily modify the InsertData module's behavior
                # We'll create a custom insertion instead of using input()
                BooksData = []
                for i in range(num_records):
                    BooksData.append(('Book title', 'test name', 2000))
                
                self.connection.executemany('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', BooksData)
                self.connection.commit()
                
                messagebox.showinfo("Success", f"Inserted {num_records} placeholder record(s)")
                self.load_data()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to insert data: {str(e)}")
    
    def add_single_book(self):
        """Add a single book with custom data"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        # Create a dialog window
        dialog = tk.Toplevel(self.root)
        dialog.title("Add New Book")
        dialog.geometry("400x200")
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Title
        ttk.Label(dialog, text="Title:").grid(row=0, column=0, padx=10, pady=10, sticky="w")
        title_entry = ttk.Entry(dialog, width=30)
        title_entry.grid(row=0, column=1, padx=10, pady=10)
        
        # Author
        ttk.Label(dialog, text="Author:").grid(row=1, column=0, padx=10, pady=10, sticky="w")
        author_entry = ttk.Entry(dialog, width=30)
        author_entry.grid(row=1, column=1, padx=10, pady=10)
        
        # Year
        ttk.Label(dialog, text="Year:").grid(row=2, column=0, padx=10, pady=10, sticky="w")
        year_entry = ttk.Entry(dialog, width=30)
        year_entry.grid(row=2, column=1, padx=10, pady=10)
        
        def save_book():
            title = title_entry.get().strip()
            author = author_entry.get().strip()
            year_str = year_entry.get().strip()
            
            if not title or not author or not year_str:
                messagebox.showwarning("Warning", "Please fill in all fields")
                return
            
            try:
                year = int(year_str)
                self.connection.execute('INSERT INTO books(title, author, year) VALUES (?, ?, ?);', 
                                      (title, author, year))
                self.connection.commit()
                messagebox.showinfo("Success", "Book added successfully!")
                dialog.destroy()
                self.load_data()
            except ValueError:
                messagebox.showerror("Error", "Year must be a valid number")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to add book: {str(e)}")
        
        # Buttons
        button_frame = ttk.Frame(dialog)
        button_frame.grid(row=3, column=0, columnspan=2, pady=20)
        
        ttk.Button(button_frame, text="Save", command=save_book).pack(side="left", padx=5)
        ttk.Button(button_frame, text="Cancel", command=dialog.destroy).pack(side="left", padx=5)
    
    def delete_selected(self):
        """Delete the selected record"""
        if not self.connection:
            messagebox.showwarning("Warning", "Please connect to a database first")
            return
        
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("Warning", "Please select a book to delete")
            return
        
        try:
            # Get the selected book's data
            item = self.tree.item(selected[0])
            book_id = item['values'][0]
            title = item['values'][1]
            
            # Confirm deletion
            confirm = messagebox.askyesno(
                "Confirm Deletion", 
                f"Delete book:\n\nID: {book_id}\nTitle: {title}\n\nAre you sure?"
            )
            
            if confirm:
                # Delete using SQL (similar to RemoveData module)
                self.connection.execute('DELETE FROM books WHERE id = ?;', (book_id,))
                self.connection.commit()
                
                messagebox.showinfo("Success", f"Book with ID {book_id} has been removed")
                self.load_data()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to delete record: {str(e)}")
    
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