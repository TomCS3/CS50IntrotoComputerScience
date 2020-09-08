// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned long N = 75000;

// Hash table
node *table[N] = {NULL};

//Size of Dictionary
int sz = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //creates a wordcopy as word is a constant
    char *wordcopy = malloc(strlen(word) + 1 * sizeof(char));
    strcpy(wordcopy, word);

    // Converts the wordcopy to lower case
    for (int i = 0; i < strlen(word); i++)
    {
        wordcopy[i] = tolower(wordcopy[i]);
    }

    // hash wordcpy to find hash index
    int hashindex = hash(wordcopy);

    //printf("text word = %s, hash = %i\n", wordcopy, hashindex);

    //if no wnode exists at the
    if (table[hashindex] == NULL)
    {
        free(wordcopy);
        return false;
    }
    else
    {
        // create a temporary node called cursor to transverse the linked list
        node *cursor = table[hashindex];

        while (cursor->next != NULL)
        {
            if (strcmp(wordcopy, cursor->word) == 0)
            {
                free(wordcopy);
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
        // if the last word in the linked list matches return true
        if (strcmp(wordcopy, cursor->word) == 0)
        {
            free(wordcopy);
            return true;
        }
        else
        {
            //cursor has reached the end of the linked list and the word has not been found
            free(wordcopy);
            return false;
        }
    }
}

// Hashes word to a number using a modified DJB2 hash function.
// First reported by Dan Bernstein Source code found at http://www.cse.yorku.ca/~oz/hash.html

unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    int hashmod = hash % N;
    return hashmod;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // opens the dictionary file
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("file failed to open\n");
        return false;
    }

    // Creates an array of characters to store the string / word
    char word[LENGTH + 1];

    // Continues to scan each word until the end of the file
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocates memory and checks for error
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Memory Error\n");
            return false;
        }

        //Copys word into node
        strcpy(n->word, word);
        n->next = NULL;

        // Call hash function to find out where the node should be placed
        int hashindex = hash(word);

        //printf("dic word = %s, hash = %i\n", word, hashindex);

        //increases the size of the dicitonary by one
        sz++;

        // If no node exists at the index then move n there
        if (table[hashindex] == NULL)
        {
            table[hashindex] = n;
        }

        // Otherwise append the node to the start of the linked list
        else
        {
            n->next = table[hashindex];
            table[hashindex] = n;
        }
    }

    // close the file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return sz;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *cursor = table[i];

            while (cursor != NULL)
            {
                // creates a temporary node that point to the same as cursor;
                node *tmp = cursor;

                //point cursor to the next node;
                cursor = cursor->next;

                // free the current temp node
                free(tmp);
            }
            //free the node cursor points to
            free(cursor);
        }
    }
    return true;
}
