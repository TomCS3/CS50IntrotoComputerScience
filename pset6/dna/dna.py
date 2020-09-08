
import sys
import csv

# displays error if the incorrect number of cmd line arguments is used
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()

# opens the dna sequence text file and converts to a string
with open(sys.argv[2], 'r') as txt:
    dna = txt.read().replace('\n', '')

# opens csv database to extract STRs
with open(sys.argv[1], 'r') as csv_file:
    csv_reader = csv.reader(csv_file)

    str_list = next(csv_reader)
    str_list.pop(0)

# creates a dictionary structure for recording the number of each STR repeated occurences in the dna sequence
str_dict = dict.fromkeys(str_list, 0)

# for each of the STR, compare it to the dna sequence to find its number of occurence
for key in str_dict:

    key_len = len(key)
    counter_max = 0

    for i in range(len(dna)):
        counter = 0
        # if the STR is found a counting process begins
        if dna[i: i + key_len] == key:
            counter += 1

            # while the STR is found to repeat a counter is increased, and i moved along
            while dna[i + key_len: i + 2 * key_len] == dna[i: i + key_len]:
                counter += 1
                i += key_len

            # keeps track of the most STR repetitions
            if counter > counter_max:
                counter_max = counter

    # stores the max number of STR Repetitions in its corresponding dictionary value
    str_dict[key] = counter_max

# creates a list of dictionarys for the dna database
with open(sys.argv[1], 'r') as csv_file:
    csv_dict_reader = csv.DictReader(csv_file, delimiter=',')

    # for each person in the database, it compares the number of STRs to the DNA sequence in question
    for line in csv_dict_reader:
        str_match = 0
        for key in str_dict:
            if str_dict[key] == int(line[key]):
                str_match += 1

        # if an exact match is found it is a match and the name is printed and the program exits
        if str_match == len(str_dict):
            print(line['name'])
            exit()

    # otherwise no exact match exists
    print("No Match")
