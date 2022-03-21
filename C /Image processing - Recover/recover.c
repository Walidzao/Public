#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char* raw_image = argv[1];
    FILE* card = fopen(raw_image,"r");

    if(card == NULL)
    {
        fprintf(stderr, "Raw file %s could not be opened.\n", argv[1]);
        return 1;
    }


    // look For the Header :
    BYTE header[3];
    do
    {
        // read three bytes
        fread(header, sizeof(header), 1, card);
        // move the cursor back two bytes
        fseek(card, -2, SEEK_CUR);
    }while(!(header[0] == 0xff && header[1] == 0xd8 && header[2] == 0xff ));

    //go back one byte to start from the first byte of the image header;
    fseek(card, -1, SEEK_CUR);

    // header detected
    // open file
    // read and write 512 bytes if the first three bytes are not a jpeg header.
    //alocate size for image name
    BYTE buffer[512];

    char* img_name = malloc(sizeof("###.jpg"));
    int img_count = 0;
    int End_reached = 0;
    do
    {
        sprintf(img_name, "%03i.jpg", img_count);
        FILE* image = fopen(img_name,"w");

        if(image == NULL)
        {
            fprintf(stderr, "%s could not be opened.\n", argv[1]);
            return 1;
        }
        int bytes_read;
        while(1)
        {

            bytes_read = fread(buffer, sizeof(BYTE), 512, card);
            if (bytes_read < 512)
            {
                printf( "end of file");
                End_reached = 1;
                break;
            }
            fwrite(buffer, sizeof(buffer), 1, image);

            // read the first triplet of the next buffer to see if it's signaling the start of a new image.
            //if a new file is detected :
            //printf("%i 1  ",(int)ftell(card));
            fread(header, sizeof(header), 1, card);
            //printf("%x %x %x  ",header[0],header[1],header[2]);
            fseek(card, -3, SEEK_CUR);

            if (header[0] == 0xff && header[1] == 0xd8 && header[2] == 0xff )
            {
                break;
            }
        }

        fclose(image);
        img_count ++;
        printf("  %i",bytes_read);
        /*int c  = 0;
        while (c < 10 && End_reached)
        {

            printf ("%x ",buffer[c]);
            c++;
        }*/

    }while(!End_reached);


    fclose(card);
    free(img_name);
}