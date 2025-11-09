import sqlite3

connection = sqlite3.connect('demo_database.db')
connection.execute('''
                    CREATE TABLE IF NOT EXISTS Students (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name Text,
                        age INTEGER
                    );
''')

connection.execute('''
                    CREATE TABLE IF NOT EXISTS Courses (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name Text,
                        Student_ID INTEGER,
                        Foreign KEY (Student_ID) REFERENCES Students(id)
                    );
''')

# insert data into the table
StudentData = [
    ('Alice', 22),
    ('Bob', 24),
    ('Charlie', 23),
    ('David', 25),
    ('Eve', 22),
]

CoruseData = [
    ('Mathematics', 1),
    ('Physics', 2),
    ('Chemistry', 3),
    ('Biology', 4),
    ('Computer Science', 5),
]

connection.executemany('INSERT INTO Students(name, age) VALUES (?, ?);', StudentData)
connection.executemany('INSERT INTO Courses(name, Student_ID) VALUES (?, ?);', CoruseData)

# query data from the table
result = connection.execute('''
                            SELECT Students.name, Courses.name
                            FROM Students                    
                            INNER JOIN Courses
                            ON Students.id = Courses.Student_ID;        
''')

# writes data to database
connection.commit()

print('Students - Course relationship')
for row in result.fetchall():
    print(f'Student Name: {row[0]}, Course Name: {row[1]}')

# closes the databases connection
connection.close()