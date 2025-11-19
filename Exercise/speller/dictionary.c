// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>   
#include <string.h>   
unsigned int cnt = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
#define N 10000000

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int i;
    char a[LENGTH + 1];
    for (i = 0; word[i] != '\0'; i++ )
    {
        a[i] = tolower(word[i]);
    }
    a[i] = '\0';
    unsigned int id = hash(a);
    node *p = table[id];
    while (p != NULL)
    {
        if(strcmp(p->word, a) == 0)
            return true;
        p = p-> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned long hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash = hash * 31 + tolower(word[i]);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *source = fopen(dictionary,"r");
    char buffer[LENGTH + 1];
    if (source == NULL)
    {
        return false;
    }
    while (fscanf(source,"%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {   fclose(source);
            return false;
        }
        strcpy(n->word, buffer);
        n->word[LENGTH] = '\0';
        unsigned int id = hash(buffer);
        n->next = table[id]; // like link the new one to the previous one like a -> by
        table[id] = n; // to update
        cnt++;
    }
    fclose(source);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return cnt;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *p = table[i];
        while(p != NULL)
        {
            node *tmp = p;
            p = p->next;
            free(tmp);
        }
    }
    return true;
}
 