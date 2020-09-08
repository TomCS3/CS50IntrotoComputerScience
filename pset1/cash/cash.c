#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars; //Float input from user
    int r; //remainder of cash when divided by the largest remaining coin
    int n; //number of coins needed for the change

    //Asks the user to input the change amount until a valid amount is inputted
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0 || dollars == 0);

    //converts float input (dollars) into and interger (cents) avoid errors
    int cents = round(dollars * 100);

    //Checks for the maximum number of quarters possible for the change
    if (cents % 25 == 0)
    {
        n = cents / 25;
    }
    else
    {
        r = cents % 25;
        n = (cents - r) / 25;
        cents = r;

        //Checks for the maximum number of dimes possible for the remaining change
        if (cents % 10 == 0)
        {
            n = n + cents / 10;
        }
        else
        {
            r = cents % 10;
            n = n + (cents - r) / 10;
            cents = r;


            //Checks for the maximum number of nickels possible for the remaining change
            if (cents % 5 == 0)
            {
                n = n + cents / 5;
            }
            else
            {
                r = cents % 5;
                n = n + (cents - r) / 5;
                cents = r;

                n = n + cents;
            }
        }
    }
    printf("%i\n", n);
}