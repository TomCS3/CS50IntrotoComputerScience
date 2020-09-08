import sys
import csv
from cs50 import SQL

# Displays correct usuage if the incorrect number of cmd line arguments is used
if len(sys.argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

# Create empty students.db
open("students.db", "w").close()

# Opens file for SQlite
db = SQL("sqlite:///students.db")

# Creates an SQL TABLE called students in  with the following headers
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# opens characters.csv and begins to read
with open(sys.argv[1], 'r') as file:

    # Creates DictReader and begins to parse row by row
    reader = csv.DictReader(file)
    for row in reader:

        # Splits the name field into first, middle and last
        full_name = row["name"].split()
        if len(full_name) == 2:
            first = full_name[0]
            middle = None
            last = full_name[1]
        else:
            first = full_name[0]
            middle = full_name[1]
            last = full_name[2]

        # Assigns house and birth values
        house = row["house"]
        birth = int(row["birth"])

        # Inserts the rows information into the database students.db
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
            first, middle, last, house, birth)


