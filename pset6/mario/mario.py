from cs50 import get_int

h = 0

# Continues to prompt the user for a height between 1 and 8 inclusive
while (h < 1 or h > 8):
    h = get_int("Height: ")

# Prints a pyramid of given height
for i in range(int(h)):
    h -= 1
    i += 1
    print(" " * h, end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i, end="")
    print("")
