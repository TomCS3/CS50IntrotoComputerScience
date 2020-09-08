from cs50 import get_int

h = 0

while (h < 1 or h > 8):
    h = get_int("Height: ")

for i in range(int(h)):
    h -= 1
    i += 1
    print(" " * h, end="")
    print("#" * i, end="")
    print("")

"""
while h > 0:
    h -= 1
    i += 1
    print(" " * h, end="")
    print("#" * i, end="")
    print("")
"""