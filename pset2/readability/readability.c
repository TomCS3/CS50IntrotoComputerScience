#include <stdio.h>
#include <cs50.h>
#include <math.h>

int count_letters(void);
int count_words(void);
int count_sent(void);

int nlet, nword, nsent, i, index;
string text;

int main(void)
{
    //Prompts user for text
    text = get_string("Text: ");
    printf("%s\n", text);

    //calls function to return the number of letter, words and sentences
    count_letters();
    count_words();
    count_sent();

    //Prints the readability of the text using the Coleman-Liau index
    index = round(100 * (0.0588 * nlet / (float) nword - 0.296 * nsent / (float) nword) - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index < 16)
    {
        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }

}


//Function returns the number of letters of the alphabet in a string
int count_letters(void)
{
    for (i = 0, nlet = 0; text[i] != '\0'; i++)
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            nlet++;
        }
    }
    return nlet;
}

//Function returns the number of words in a string
int count_words(void)
{
    if ((text[0] >= 65 && text[0] <= 90) || (text[0] >= 97 && text[0] <= 122))
    {
        for (i = 0, nword = 1; text[i] != '\0'; i++)
        {
            if (text[i] == 32)
            {
                nword++;
            }
        }
    }
    else
    {
        nword = 0;
    }

    return nword;

}

//Function returns the number of sentences
int count_sent(void)
{
    for (i = 0, nsent = 0; text[i] != '\0'; i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            nsent++;
        }
    }
    return nsent;
}