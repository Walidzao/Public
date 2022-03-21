// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
uint8_t header[HEADER_SIZE];
int16_t buffer;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // open a new file which will be the destination scaled Wav file. Name will be argv[2].
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // factor is the scale factor in the type float.
    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    //assuming the header is the start of the fil, read and write the header to the output wav file
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);

    // Copy after scaling the buffer to the wav output.
    int16_t scaled_buffer;
    while (fread(&buffer, sizeof(int16_t),1,input))
    {
        scaled_buffer = buffer * factor;
        fwrite(&scaled_buffer, sizeof(int16_t), 1, output);
    }


    // TODO: Read samples from input file and write updated data to output file

    //free the allocated memory from the heap.


    // Close files
    fclose(input);
    fclose(output);
}
