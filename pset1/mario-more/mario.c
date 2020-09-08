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

    //First loop dictates the number of rows and intiates them one by one
    for (int i = h; i > 0; i--)
    {
        //Secondary loops build each row, space by space and block by block
        for (int j = (i - 1); j > 0; j--)
        {
            printf(" ");
        }
        for (int k = (h - (i - 1)); k > 0; k--)
        {
            printf("#"); 
        } 
        printf("  ");
        for (int k = (h - (i - 1)); k > 0; k--)
        {
            printf("#"); 
        }
        printf("\n");
    }
}