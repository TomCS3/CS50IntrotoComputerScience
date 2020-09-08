#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long long number, c1, c2;
    int sum1, sum2, chksum;

    //prompts user for credit card number (must be positive and off at least 1 digit)
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    //quick check of validilty of the card number - min and max values
    if (number < 3400000000000 || number > 5599999999999999)
    {
        printf("INVALID\n");
    }

    else
    {
        //sum of every other digit of the number starting from the last digit
        for (c1 = number, sum1 = 0; c1 > 0; c1 /= 100)
        {
            sum1 += c1 % 10;
        }

        //Multiplies every other digit by 2, starting with the second to last digit
        //Subsequently adding those products' digits together
        for (c2 = number / 10, sum2 = 0; c2 > 0; c2 /= 100)
        {
            int prod = c2 % 10 * 2;
            if (prod > 9)
            {
                sum2 += prod / 10 + prod % 10;
            }
            else
            {
                sum2 += prod;
            }

        }

        //adds the two sums together to create the checksum
        chksum = sum1 + sum2;

        //if the last digit of chksum is zero, the credit card type is then determined
        if (chksum % 10 == 0)
        {
            if (number > 5099999999999999 && number < 5600000000000000)
            {
                printf("MASTERCARD\n");
            }
            else if (number / 1000000000000000 == 4 || number / 100000000000 == 4)
            {
                printf("VISA\n");
            }
            else if (number / 10000000000000 == 34 ||  number / 10000000000000 == 37)
            {
               printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }

        else
        {
            printf("INVALID\n");
        }
    }
}