#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calculate_letter_pos(char c);
int main(int argc, string argv[])
{

//Verify validity of the user input.
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
// check that all the characters are alphabetic
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("The key should only have letters");
            return 1;
        }
        for (int j = 0; j < i; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Usage: The key should not have a same letter twice ! ");
                return 1;
            }
        }
    }

// The user input at this point is congruent to the specs.

// Demand user input for the text to be crypted by the key
    string text = get_string("plaintext:  ");
    int n = strlen(text);
//Alternative declaration like the following : char[strlen(text)] cypherText;
    char cypherText[n + 1];


//The position of a given letter in the alphabet
    int position;

    for (int i = 0 ; i < n; i++)
    {
        if (isalpha(text[i]))
        {
        // Get the position of the letter
            position = calculate_letter_pos(text[i]);
        //Retrive the letter of the same position in the key.
            if (isupper(text[i]))
            {
                cypherText[i] = toupper(argv[1][position]);
            }
            else
            {
                cypherText[i] = tolower(argv[1][position]);
            }
        }
        else
        {
            cypherText[i] = text[i];
        }
    }

// print the cypher text
    cypherText[n] = '\0';
    printf("ciphertext: %s\n", cypherText);
    }


int calculate_letter_pos(char c)
{
    if (islower(c))
    {
        return c - 'a';
    }
    else
    {
        return c - 'A';
    }
}