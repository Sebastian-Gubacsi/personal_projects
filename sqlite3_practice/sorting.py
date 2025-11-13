import sqlite3

connection = sqlite3.connect('demo_database.db')
connection.execute('''
                    CREATE TABLE IF NOT EXISTS Employees (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name Text,
                        age INTEGER,
                        Department Text
                    );
''')

# insert data into the table
EmployeeData = [
    ('John Doe', 30, 'HR'),
    ('Jane Smith', 25, 'Finance'),
    ('Emily Davis', 38, 'IT'),
    ('Michael Brown', 45, 'Marketing'),
    ('Jessica Wilson', 29, 'Sales'),
]

# connection.executemany('INSERT INTO Employees(name, age, Department) VALUES (?, ?, ?);', EmployeeData)

# query data from the table
result = connection.execute('SELECT * FROM Employees ORDER By age DESC;') # ASC is ascending order/ DESC is descending order
data = result.fetchall()

# display queried data
print('Employees in Database:')
for row in data:
    print(f'Name: {row[1]}')
    print(f'Age: {row[2]}')
    print(f'Department: {row[3]}')
    print('')

# writes data to database
connection.commit()

# closes the databases connection
connection.close()