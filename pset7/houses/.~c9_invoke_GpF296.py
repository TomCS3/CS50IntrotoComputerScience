import sys
import csv
from cs50 import SQL

# Displays correct usuage if the incorrect number of cmd line arguments is used
if len(sys.argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Opens file for SQlite
db = SQL("sqlite:///students.db")

rows = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])

for rows in rows:
    if row["middle"] == None:
        print(f'{row[]})