#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);


    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins !\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins !\n");
    }
    else
    {
        printf("Tie !");
    }
}

int compute_score(string word)
{
    //index of the of a given letter in the alphabet. Will be used to get the scrabble score of the letter from the POINTS Arraym
    int index;
    //initialze the return value : scrabble score to 0
    int score = 0;

    // TODO: Compute and return score for string
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isalpha(word[i]))
        {
            index = toupper(word[i]) - 'A';
            score += POINTS[index];
        }
    }
    return score;
}