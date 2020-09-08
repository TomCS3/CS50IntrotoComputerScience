#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //assigns h to the height inputted by the user
    int h;

    //continues to prompt user until a positive number between 1 and 8 is entered
    do
    { 
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    //double loop to build blocks, first loop builds the rows, second loop builds the columns
    for (int i = h; i > 0; i--)
    {
        for (int j = (i - 1); j > 0; j--)
        {
            printf(" ");
        }
        for (int k = (h - (i - 1)); k > 0; k--)
        {
            printf("#");  
        }
        printf("\n");
    }
}