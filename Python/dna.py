import cs50
from sys import argv
import csv

sequence = ''

def main() :
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    elif len(argv) == 3 :
        data = argv[1]
        dna_file = argv[2]

        dictionary = []
        #  read the CSV files  into a dictionarry array"""
        with open(data, "r") as data_file:
            csv_reader = csv.DictReader(data_file)
            for row in csv_reader:
                dictionary.append(row)
        #print(dictionary)
        # retrive the genome substrings """
        genomes = list(dictionary[0].keys())
        genomes = genomes[1:]

        # read the dna sequence from text file and convert it into a string for ease of manipulation """

        with open(dna_file, "r") as text:
            for c in text:
                global sequence
                sequence += c

        #Create a dictionary of genome: Str pairs in the given sequence.
        genome_count = {}
        for genome in genomes :
            genome_count[genome] = str_count(genome)

        found = True
        for person in dictionary:
            found = True
            for genome in genome_count:
                if genome_count[genome] != int(person[genome]):
                    found  = False
                    break
            if found == True:
                #Person found
                print(person["name"])
                break

        if found == False:
            print( "No match")

def str_count(Str) :

    index = sequence.find(Str)
    #print(index)
    #verifiy how many back to back occurences substring
    count, count2 = 0, 0
    if index != -1:   #found
        count = 1
        count2 = 1
        subseq = sequence[index + len(Str):]
        while len(subseq) > len(Str) :
            if Str == subseq[:len(Str)] :
                #print('# consecutive str found')
                count += 1
                subseq = subseq[len(Str):] # slice from the end of the consecutive str.
            else: # End of a sequence : the next str doesn't match the one before.
            #verify if more than one sequence has been found : in which case we are gonna keep as count the bigger
                count2 = max (count, count2)
                count = 1

                index = subseq.find(Str)
                if index != -1:

                    # New subsequence found => to work with.
                    subseq = subseq[index + len(Str) :]

                else : # No sequence ahead // sequence ended => return the count.
                    break
    return max(count, count2)


if __name__ == "__main__":
    main()