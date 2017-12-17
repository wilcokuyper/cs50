/**
 * Implements a dictionary's functionality.
 */
 
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

// define max length of each trie "layer" 26 letter + '\''
#define NODE_LENGTH 27
#

// define trie node
typedef struct node
{
    bool is_word;
    struct node *children[NODE_LENGTH];
}
node;

// global variables to hold the word count and the root node
int wordcount = 0;
node *root;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // create a current node and set it to the root elemend
    node * current = root;
    
    // iterate over the characters in the word
    int i = 0;
    while(*(word + i) != '\0')
    {
        // calculate letter index, apostrophe = last character;
        int index;
        if(*(word + i) == '\'')
            index = NODE_LENGTH - 1;
        else
            index =  tolower(*(word + i)) - 'a';
        
        // if node for current character exists make that the current node and move to the next character    
        if (current->children[index] != NULL)
        {
            current = current->children[index];
            i++;
        }
        else 
        {
            // character does not exist in the dictionary so the word is misspelled
            return false;
        }
    }
    
    // end of word reached so the wordt must be spelled correctly
    return current->is_word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // try to open the dictionary
    FILE* fp = fopen(dictionary, "r");
    if(fp == NULL)
        return false;

    // initialize root node and set the current node to it
    root = calloc(1, sizeof(node));
    node *current = root;

    // read the whole file character by character
    char c;
    while( (c = fgetc(fp)) != EOF)
    {
        // the the character is not a newline 
        if (c != '\n')
        {
            // calculate the index of the character, aposthophe is the last character
            int index;
            if (tolower(c) == '\'')
                index = NODE_LENGTH -1;
            else
                index = tolower(c) - 'a';

            // if a node for the current character does not exist create it
            if (!current->children[index])
                current->children[index] = calloc(1, sizeof(node));
            
            // set the current node to the child node corresponding to the current character
            current = current->children[index];
        }
        else
        {
            // we have react the end of the word, set the current node's word flag to true and increment the word count
            current->is_word = true;
            wordcount++;
            current = root;
        }
    }
    // close the dictionary
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordcount;
}

bool freeNode(node *n)
{
    // loop over n children recursively freeing each one.
    for(int i = 0; i < NODE_LENGTH; i++)
    {
        if (n->children[i] != NULL)
            freeNode(n->children[i]);
    }
    // node children found, free the current node
    free(n);
    return true;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    return freeNode(root);
}
