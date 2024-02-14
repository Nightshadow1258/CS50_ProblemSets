// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

void print_hashtable(void);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hashindex = hash(word);
    node* temp = table[hashindex];
    while(temp != NULL)
    {
        char * word_comp = temp->word;
        word_comp[0] = tolower( (int) word_comp[0]); // that did the trick, not sure why the  first letter is not handles case-insnsitive
        if (strcasecmp(word,word_comp) == 0) //match found
        {
            return true;
        }
        temp = temp->next;

    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // // TODO: Improve this hash function
    // int sum = 0;
    // for (int i = 0; word[i]  != '\0';i++)
    // {
    //     sum += word[i];
    // }
    // return sum % (N+1);

    // source for this: https://stackoverflow.com/questions/7700400/whats-a-good-hash-function-for-english-words
    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % (N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    char word[LENGTH + 1];
    char character = '\0';
    int counter = 0;
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Could not open dictionary: %s", dictionary);
        return false;
    }

    while (fread(&character, sizeof(char), 1, source))
    {

        if (character == '\n') // word finished
        {

            // terminate string
            word[counter] = '\0';
            // calcualte hash value
            int hashindex = hash(word);
            // create new node and add to table
            node* new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                return 1;
            }
            strcpy(new_node->word, word);
            new_node->next = NULL;


            node *current = table[hashindex];
            // base case
            if (table[hashindex] == NULL) // first element
            {
                table[hashindex] = new_node;
                word[0] = 0; // reset string. dirty move other indices are still set
                counter = 0; // reset counter
                continue;
            }
            else
            {
                // find last node
                while (current->next != NULL)
                {
                    current = current->next;
                    //printf("%s\n", current->word);
                }
                current->next = new_node;
                word[0] = 0; // reset string. dirty move other indices are still set
                counter = 0; // reset counter
                continue;
            }

        }
        word[counter] = character;
        counter++;
    }

    //print_hashtable();
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    long counter = 0;
     for (int i = 0; i < N; i++)
    {
        node* hash = table[i];
        while(hash != NULL)
        {
            hash = hash->next;
            counter++;;
        }
    }
    //printf("Number of words in dictionary: %ld",counter);
    // large dictionary has 143091 entries
    //  small dictionary has 2  entries
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node* temp = table[i];
        while(table[i] != NULL)
        {
            temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }

    }
    
    return true;
}

void print_hashtable(void)
{
    for (int i = 0; i < N; i++)
    {
        node* hash = table[i];
        printf("Hashtable %i:\n", i);
        while(hash != NULL)
        {
            printf("%s, ", hash->word);
            hash = hash->next;
        }
        printf(".\n");
    }
}