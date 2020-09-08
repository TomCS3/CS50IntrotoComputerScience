import sys
from cs50 import SQL

# Displays correct usuage if the incorrect number of cmd line arguments is used
if len(sys.argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Opens file for SQlite
db = SQL("sqlite:///students.db")

# Selects the students from the given house from the SQL database
rows = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])

# Prints the name and birth of each student in the house
for row in rows:
    if row["middle"] == None:
        print(f'{row["first"]} {row["last"]}, born {row["birth"]}')
    else:
        print(f'{row["first"]} {row["middle"]} {row["last"]}, born {row["birth"]}')