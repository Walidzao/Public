#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    // insert the names of each candidate in the array of candidates, and intilize votes to 0.
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    // check if the name figures in the list, if positive increment the vote by 1.
    for ( int i = 0; i < MAX; i++)
    {
        if (candidates[i].name == NULL)
        {
            break;
        }

        else if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes += 1;
            return true;
        }
    }

    //else return false.
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Itterate through thez whole struct finding the biggest vote
    int n = 0;
    int highest_vote = 0;

    //First itteration to find the biggest vote in the list
    while(n < MAX)
    {
        if (candidates[n].votes > highest_vote)
        {
            highest_vote = candidates[n].votes;
        }
        n++;
    }

    //Second itteration : if the candidates votes equales the highest vote
    //print the candidates name and print a new line
    for (int i = 0; i < MAX; i++)
    {
        if (candidates[i].votes == highest_vote)
        {
            // print the candidates name.
            printf ("%s\n",candidates[i].name);
        }
    }

    return;
}

// note make sure we have 4 valid votes. do not count the ballots / segnmentation fault dur to the search of blank zone