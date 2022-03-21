#include <cs50.h>
#include <stdio.h>

int main(void)
{

    //  Declare the variables, years needed, growing size, starting size of the population and the endind size
    float size;
    int years, startSize, endSize;

    // TODO: Prompt for start size
    do
    {
        startSize = get_int(" Hy, What is the start size of the population ( 'hint : positive number ' )");
    }
    while (startSize < 9);


    // TODO: Prompt for end size
    do
    {
        endSize = get_int(" And,what is the desired end size of the population ");
    }
    while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold

    // give size an init value
    size = startSize ;
    years = 0;


    // do a while loop that eep multipliying the output by 13 / 12 anf if we reach that number we exit the loop eventually printing the number.

    while (size < endSize)
    {

        size = size + (int)(size / 3.0) - (int)(size / 4.0);

        years ++;
    }


    // TODO: Print number of years

    printf("Years: %i", years);

}