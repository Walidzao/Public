#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j. 3 people prefer candidate i/1 over candidate j/2.
// So preferences[1][2]= 3 // preferences[2][1]= 4
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];


// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates array of the candidate of type string.
string candidates[MAX];


// a matrix of type pair with a number of maximun possible pairs, Max candidates is 9.
//Pairs[0].winner will output the winner candidate index of the second pair.
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(pair *a, pair *b);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        // populating the candidate string matrix.
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    // initialize the pair count to zero
    pair_count = 0;

    //demand the number of voters.
    int voter_count = get_int("Number of voters: ");

    // Query for votes, through the voters numbers.
    for (int i = 0; i < voter_count; i++)
    {
        // Ranks[i] is voter's ith preference
        // For each voter will be an array ranks, rank[O] will be the voter's first preference.
        int ranks[candidate_count];

        // Query for each voter // rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            // name is the candidate's name of j'th preference.

            // vote will take the candidate name and compare it to the candiate string array,
            // outputing false if it doesn't find a match, and quitting the program.
            // vote will take an array of strings as well.
            // in ranks[j] = Index of candidate "name"
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }


        // Ranks will be replaced by the ranks of the second voter, thus recoding it's values by record_preferences.
        // update the global preferences variable.
        // itterate through the array through an pref[rank[i]][rank[j]]++
        // i from 0 to the last minus 1 and j from i plus 1 to the last.


        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // look in the candidates array for name, create a loop to return the index of the candifdate once we find it.

    int i = 0;

    while (i < candidate_count && strcmp(candidates[i], name) != 0)
    {
        i++;
    }

    //if we have itterated through the entire array without finding a match with candidate's name, we will return false.
    if (i == candidate_count)
    {
        return false;
    }
    // otherwise we will return the index of the candidate's name as the voter's j'th preference rank.
    else
    {
        ranks[rank] = i;
        return true;
    }

}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // we will make usage of the ranks array, to update the preferences [i][j].

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (preferences[i][j] == preferences[j][i])
            {

            }
            else
            {

                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                if (preferences[i][j] < preferences[j][i])
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }

                //increment the number of winner/loser pairs.
                pair_count ++;
            }
        }
    }


    return;
}


//swap function created here.
void swap(pair *a, pair *b)
{
    pair temp;

    temp.winner = (*a).winner;
    temp.loser = (*a).loser;

    (*a).winner = (*b).winner;
    a->loser = b->loser;

    b->winner = temp.winner;
    b->loser = temp.loser;

    return;
}



// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    int score_j, score_j1;
    int swaps = 0;
    int k = 0;
    // implement bubble sort.
    // the swap function is correct.
    // the j and k delimiters are perhaps faulty.



    do
    {
        swaps = 0;
        for (int j = k; j < pair_count - 1; j++)
        {
            score_j  = preferences[pairs[j].winner][pairs [j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            score_j1 = preferences[pairs[j + 1].winner][pairs [j + 1].loser] - preferences[pairs[j + 1].loser][pairs[j + 1].winner];

            if (score_j < score_j1)
            {

                swap(&pairs[j], &pairs[j+1]);
                swaps++;
            }
        }
        k++;


    }while (swaps > 0);

    return;
}



//recursive function to search for a lock path from a to b
bool islocked(int a, int b)
{
    if (locked[a][b])
    {
        return true;
    }
    else
    {
        int x = 0;
        while (x < candidate_count)
        {

            if (x == a || x == b)
            {
                x++;
            }
            // should we verifiy if x != candidate_count ?
            else
            {
                // the candidate number x is not a possible intemidiary candidate in a cycle
                if (!(locked[a][x]))
                {
                    x++;
                }

                else
                {
                    if (locked[x][b])
                    {
                        return true;
                    }
                    else if (islocked(x, b))
                    {
                        return true;
                    }
                    else
                    {
                        x++;
                    }
                }
            }
        }
        return false;
    }
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Keep locking until we reach a cycle, in which case we will return void.
    for (int i = 0; i < pair_count; i++)
    {
        if (!(islocked(pairs[i].loser, pairs[i].winner)))
        {
            locked[pairs[i].winner] [pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
// Print the candidate that doesn't have an arrow pointed
void print_winner(void)
{

    for(int j = 0; j < candidate_count; j++)
    {
        int k;
        for(k = 0; k < pair_count; k++)
        {
            // is the pair k is a locked pair.
            // we check is candidate k is the loser of the pair, meaning that an arrow will be pointing at him
            if (locked[pairs[k].winner] [pairs[k].loser] && pairs[k].loser == j) // j is not the winning candidate.
            {
                // k will be the pair in which candidate j loses.

                break;
            }

        }
        // two options are presented
        // 1 st : we have reached the end of the pairs. and k is equal to pair count
        // it is possible that j is the loser of the pair number pair_count -1
        if (k == pair_count)
        {
            // the candidate is not the loser candidate in any locked pair.
            //candidate j is the winner.
            printf("%s\n",candidates[j]);
            return;
        }
    }



    }


