#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //if the wrong number of cmd line arguments are given it informs the usr how to exceute correctly
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    else
    {
        //if the key consists of any non digits, it informs the usr how to execute correctly
        string key = argv[1];
        for (int i = 0; key[i] != '\0'; i++)
        {
            if (key[i] < '0' || key[i] > '9')
            {
                printf("Usage: ./caesar key\n");
                return 1;

            }
        }

        //converts the given cmd line argument for the key into an integer store as k
        int k = atoi(argv[1]);

        //prompts the user for the text to encrypt
        string text = get_string("plaintext: ");

        //converts the plaintext into the cipher for both upper and lower cases
        for (int i = 0; text[i] != '\0'; i++)
        {
            if (text[i] >= 65 && text[i] <= 90)
            {
                text[i] = (text[i] - 65 + k) % 26 + 65;
            }
            else if (text[i] >= 97 && text[i] <= 122)
            {
                text[i] = (text[i] - 97 + k) % 26 + 97;
            }
        }

        //prints the encrypted text and returns 0 showing it is succesful
        printf("ciphertext: %s\n", text);
        return 0;
    }
}