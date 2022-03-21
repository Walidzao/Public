// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    //String and a pointer to the next element.
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 14900787; // 14900786 value of the hash function of the string "zzzzz"

// Hash table
// Decalare an array of size N of nodes each of which will contain a string and a pointer to another node.
node *table[N];

//Global variable initialized to Zero by default.
int word_counter;

// Returns true if word is in dictionary, else false
//
bool check(const char *word)
{
    // TODO
    // Get the index of the table that contains the linked list that the word should be part of.
    bool found = false;
    int diff;
    int n = hash(word);
    node * list = table[n];
    if(list == NULL)
    {
        // word doesn't exit since the pointer hto the linked list didn't get intialized.
        return found;
    }
    int len = strlen(word);
    if(len < 5)
    {
        if(table[n] != NULL)
        {
            diff = strcasecmp(list->word, word);
            if(diff == 0)
            {
                found = true;
            }
        }
    }
    else
    {
        /* The word in the linked list is greater in the order of ASCII compared to the text word*/
        // the text word might be on the next nodes of the linked list
        // since the next words are greater in terms of their alphabetical order.
        do
        {
            diff = strcasecmp(word, list->word);
            if( diff > 0)
            {
                //update the list pointer
                list = list->next;
            }
            else if(diff == 0)
            {
                found = true;
            }
            else
            {
                // diff < 0; if the text word is in the dictionary
                //it should have been found in the previous words of the linked list
                //we overpassed the word and it has not been found in the dictionary
                // example : text word : abondonem // linked list words : abandonee, abandoner

                continue;
            }
        }while(!found && list != NULL && diff > 0);
    }

    return found;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // compute the lenght of the string.
    //
    int word_lenght = (strlen(word) < 6) ? strlen(word) : 5;
    int n = 0, hash = 0;

    for(int i = 0; i < word_lenght; i++)
    {
        //n is the index of the alphabet letter.
        if(isalpha(word[i]))// A or a will return 1, Z will retun 26
        {
            n = toupper(word[i]) - 64;
        }
        else if(word[i] == 39)//apostrophe will return 0.
        {
            n = 0;
        }

        hash += pow(27, word_lenght - i - 1)*(n + 1);
    }

    // for mathematical considerations we shoudl return hash minus 1
    return hash -1;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dict)
{
    // TODO
    // dictionnary will be be the pathname of the dictionary to be openned
    FILE *dictionary = fopen(dict, "r");
    if(dictionary == NULL)
    {
        return false;
    }
    // we should read from the dictionary and load it into the Hash table following the design that we specified.
    int index;
    //retun value of the hash function, index of the hash table.
    int n;
    char word[LENGTH + 1];
    char c = fgetc(dictionary);
    node *tmp, *wordnode;
    while(!feof(dictionary))
    {
        index = 0;
        //could use fscanf to ge t
        while(c != '\n' && c!= EOF)
        {
            word[index] = c;
            index ++;
            c = fgetc(dictionary);
            if(ferror(dictionary))
            {
                return false;
            }
        }
        word[index] = '\0';
        c = fgetc(dictionary);
        //copied a whole word into the char array word.
        //alocate a memory for a new node pointer.
        // check whether the declaration of a varaible with the same name is memory consuming.
        wordnode = malloc(sizeof(node));
        if(wordnode == NULL)
        {
            return false;
        }
        //copy word into the word variable of new;
        strcpy(wordnode->word, word);
        wordnode->next = NULL;

        //increment the number of words added from the dictionary.
        word_counter ++;
        //retrieve the hash table index
        n = hash(word);

        //insert node of word in the end of the linked list starting from the node pointer table[n]

        tmp = table[n];
        if(tmp == NULL)
        {
            table[n] = wordnode;
        }
        else
        {
            while(tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            tmp->next = wordnode;
        }
    }
    fclose(dictionary);
    // we have itterated throught the whole file. char by char.

    return true;
    // Return true eventually if all elements of the dictionary are inputed into the hash table.
    // So basically before we design the hash table
    //itterate the file looking for every word, and malloc a node at every occurance, and insert it in the linked list
    // We could use int number =  hash(word);
    // And then insert in the linked list.
    // insert in table[number];
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // itterate through the table/
    // if the element is not null, apply the free linked list function which will proceed as follows
    // the first pointer will point to next
    node * list;
    node *tmp = NULL;
    for(int i = 0; i < N; i++)
    {
        list = table[i];
        while(list != NULL)
        {
            //Proceed to free the linked list

            tmp = list->next;
            free(list);
            list = tmp;
        }
        free(list);
    }
    return true;
}
