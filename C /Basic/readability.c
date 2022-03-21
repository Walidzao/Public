#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_sentences(string text);
int counts_words(string text);
int main(void)
{
    // Get user input
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int sentences = count_sentences(text);
    int words = counts_words(text);

    // calculate Coleman-Liau index
    float L = ((float)letters / words) * 100, S = ((float)sentences / words) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Print readability grade accordinly
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}

// count letter : itterate until NUL carracter, increment if the carracter is of alpha type.
int count_letters(string text)
{
    int i = 0, letters = 0;
    while (text[i] != '\0')
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        i++;
    }
    return letters;
}

// count sentences, is the carracter is . or a ! or a ? increment the counter.
int count_sentences(string text)
{
    int i = 0, sentences = 0;
    while (text[i] != '\0')
    {
        switch (text[i])
        {
            case '!':
            case '?':
            case '.':
                sentences ++;
                i++;
                break;
            default:
                i++;
        }
    }
    return sentences;
}

// count the words in the text
int counts_words(string text)
{
    int words = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (text[i] == ' ')
        {
            words++;
            while (!isalpha(text[i+1]) && i+1 < n)
            {
                i++;
            }
        }
    }
    //the total number of word will be equals to the counts of spaces plus the last word since it not follwed by a space.
    if (text[n-1] == ' ') return words;
    else
    {
      return words + 1;
    }

}