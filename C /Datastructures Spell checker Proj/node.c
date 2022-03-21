    #include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct node
{
    int number;
    struct node *next;

}
node;

int main(int argc, char *argv[])
{
    node *list = NULL;
    node *n = malloc(sizeof(node));
    if( n == NULL)
    {
        return 1;
    }
    n->number = 1;
    n->next = NULL;

    list = n;

    n = malloc(sizeof(node));
    if(n == NULL)
    {
        free(list);
        return 1;
    }
    n->number = 2;
    n->next = NULL;
    list->next = n;

    n=malloc(sizeof(node));
    if(n == NULL)
    {
        free(list);
        free(list->next);
        return 1;
    }

    n->number = 3;
    n->next = NULL;
    list->next->next = n;

    for(node *tmp = list; tmp != NULL; tmp = tmp->next)
    {
        printf("%i",tmp->number);
    }

    // code to free the whole linked list

    while(list != NULL)
    {
        node *tmp = list->next;
        free(list);
        list = tmp;
    }
}