#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>

int key_alpha(void);
string key;

int main(int argc, string argv[])
{
    //checks for the correct numnber of command line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    key = argv[1];

    //checks for key validility and returns 1 if key is invalid
    if (key_alpha() == 1)
    {
        return 1;
    }

    //prompts the user for the text to encrypt
    string text = get_string("plaintext: ");

    //converts the plaintext into the cipher for both upper and lower cases
    for (int i = 0, x = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            x = (text[i] % 65);
            if (key[x] >= 'A' && key[x] <= 'Z')
            {
                text[i] = key[x];
            }
            else
            {
                text[i] = (key[x] - 32);
            }
        }
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            x = (text[i] % 97);
            if (key[x] >= 'a' && key[x] <= 'z')
            {
                text[i] = key[x];
            }
            else
            {
                text[i] = (key[x] + 32);
            }
        }
    }

    //prints the encrypted text and returns 0 showing it is succesful
    printf("ciphertext: %s\n", text);
    return 0;
}



//checks for an valid alphabetical key of the correct length and using each letter once
int key_alpha(void)
{
    if (key[26] != '\0')
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    int ascii_total = 0;

    for (int i = 0; key[i] != '\0'; i++)
    {
        if (key[i] >= 'A' && key[i] <= 'Z')
        {
            ascii_total += (key[i] - 64);
        }
        else if (key[i] >= 'a' && key[i] <= 'z')
        {
            ascii_total += (key[i] - 96);
        }
        else
        {
            printf("Key must contain only alphabetical characters\n");
            return 1;
        }
        if (key[i] == key[i - 1])
        {
            printf("Key must not contain duplicate characters\n");
            return 1;
        }
    }
    //checks for using each letter once
    if (ascii_total != 351)
    {
        printf("Key must only contain each letter of the alphabet once\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
