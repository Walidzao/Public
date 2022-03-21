import cs50
import sys

#compure from a given string the coleman-Liau formula : 0.0588 * L - 0.296 * S - 15.8 / L average letters per 100 words 

text = cs50.get_string("Text: ")

# compute L : average number of letters per 100 words : calculate letter in the text and count the number of words.
# compute S : average number of sentences per 100 words => calculate sentences divide by number of words and multiply by 100. 
letters, sentences, words = 0, 0, 0

for c in text: 
    if c.isalpha():
        letters += 1
    if c == " ":
        words += 1
    if c in ["?", ".", "!"]:
        sentences += 1
        
#account for the last word.
words += 1

L = (letters / words) * 100
S = (sentences / words) *100

formula = round(0.0588 * L - 0.296 * S - 15.8)

if formula < 1: 
    print("Before Grade 1")
elif formula in range(1, 16):
    print("Grade", formula)
else: 
    print("Grade 16+")