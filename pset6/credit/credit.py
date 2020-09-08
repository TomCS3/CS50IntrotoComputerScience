from cs50 import get_int

number = 0

# Prompt user for a credit card number
while number < 1:
    number = get_int("Number: ")

# Quick check of validilty of the card number - min and max values
if (number < 3400000000000 or number > 5599999999999999):
    print("INVALID")

else:
    c1, c2 = number, number // 10
    sum1, sum2 = 0, 0

    # Sum every other digit of the number starting from the last digit
    while c1 > 0:
        sum1 += c1 % 10
        c1 //= 100

    # Multiplies every other digit by 2, starting from the 2nd last digit, and adds the products digits
    while c2 > 0:
        prod = c2 % 10 * 2
        if (prod > 9):
            sum2 += prod // 10 + prod % 10
        else:
            sum2 += prod
        c2 //= 100

    # Creates the checksum
    chksum = sum1 + sum2

    # If the last digit of checksum is zero the card is valid
    if (chksum % 10 == 0):
        if (number > 5099999999999999 and number < 5600000000000000):
            print("MASTERCARD")
        elif (number // 1000000000000000 == 4 or number // 100000000000 == 4):
            print("VISA")
        elif (number // 10000000000000 == 34 or number // 10000000000000 == 37):
            print("AMEX")
        else:
            print("INVALID")

    else:
        print("INVALID")