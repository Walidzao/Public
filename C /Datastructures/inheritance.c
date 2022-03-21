// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;


//Pointer to the childor generation 0; which will point to the parent generations.
person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    // passing in the number of generation wanted by passing a constant global variable
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // TODO: Allocate memory for new person

    person *individual = malloc(sizeof(person));

    // Generation with parent data

      // Generation with parent data
    if (generations > 1)
    {
        // TODO: Recursively create blood type histories for parents

        individual->parents[0] = create_family(generations -1);
        individual->parents[1] = create_family(generations - 1);
        // TODO: Randomly assign child alleles based on parents
        //at this point both parents are created
        // inherit halleles form individual->parent[i]->allele[0/1];
        individual->alleles[0] = individual->parents[0]->alleles[rand() % 2];
        individual->alleles[1] = individual->parents[1]->alleles[rand() % 2];
    }

    // Generation without parent data
    else
    {
        // TODO: Set parent pointers to NULL
        individual->parents[0] = NULL;
        individual->parents[1] = NULL;
        // TODO: Randomly assign alleles
        individual->alleles[0] = random_allele();
        individual->alleles[1] = random_allele();
    }

    // TODO: Return newly create
    return individual;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // TODO: Handle base case
    // if we lose the first pointer to the faily, generation 0 we alost the rest of the family
    // create pointer* tmp; and assign the value of person to it, and the free person, and try to free both parents recursively,
    // base case would be if the parents are NULL
    if(p == NULL)
    {
        return;
    }


    // TODO: Free parents
    // lets try to decrlare a recursive call to free the parents via the same function,

    free_family(p->parents[0]);
    free_family(p->parents[1]);


    // TODO: Free child
    free(p);

}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
