#include "helpers.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE average;
    for(int i = 0; i < height; i++)
    {
        for( int j = 0; j < width; j++)
        {

            average = round( (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0 );
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //swap rgbTriples left to right
    //creating a temprary RGB triple to store the value of the pixel.
    // swapping the element j with the element width - 1 - j.

    RGBTRIPLE temp;

    for(int i = 0; i < height; i++)
    {
        for( int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];

            image[i][j] = image[i][width -1 -j];

            image[i][width -1 -j] = temp;
        }

    }
    return;
}


void blur_line (int i, int height, int width, RGBTRIPLE row2[width], RGBTRIPLE image[height][width])
{
    RGBTRIPLE var1;
    RGBTRIPLE var2;
    WORD blue = 0, green = 0, red = 0;



    int counter = 0;
    for(int a = 0; a < 2; a++)
    {
        for(int b = 0; b < 3; b++)
        {
            if(i + b >= 0 && i + b < height && a < width ) // verify conditions of a.
            {

                //calculate the pixel image[i][j+1]


                blue += image[i + b][a].rgbtBlue;

                green += image[i+ b][a].rgbtGreen;
                red += image[i+ b][a].rgbtRed;
                counter ++;
            }
        }
    }


    blue = round(blue / (float)counter);
    green = round(green / (float)counter);
    red = round((float)red / counter);

    var2.rgbtBlue = blue;
    var2.rgbtGreen = green;
    var2.rgbtRed = red;


    for(int j = 0; j < width ; j++)
    {

        var1 = var2;

        //Calculate the bloc of j + 1 and put it in var2

        counter = 0;
        blue = 0;
        green = 0;
        red = 0;

        for(int a = 0; a < 3; a++)
        {
            for(int b = 0; b < 3; b++)
            {
                if((i+b >= 0 && i+b < height) && (j + a >= 0 && j + a < width))
                {
                    //calculate the pixel image[i][j+1]

                    blue += image[i + b][j + a].rgbtBlue;
                    green += image[i+ b][j + a].rgbtGreen;
                    red += image[i+ b][j + a].rgbtRed;
                    counter ++;
                }
            }
        }


        blue = round(blue / (float)counter);
        green = round(green / (float)counter);
        red = round((float)red / counter);

        var2.rgbtBlue = blue;
        var2.rgbtGreen = green;
        var2.rgbtRed = red;

        // image[i][j] = var1;
        row2[j] = var1;
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // 1 itterate over the whole row computing the blured values for each pixel => store in array of type RGBTRIPLE pixel_1[width];

    //declare two rows of size width of type RGBTRIPLE.
    RGBTRIPLE row1[width], row2[width];


    blur_line(-1, height, width, row2, image);

    for(int i = 0; i < height; i++)
    {

        // get a copy of row2 in row 1 and proceed ot calculate row2 before actualy modifying the values of image[i] ith row 1 values;
        memcpy(&row1, &row2, sizeof(row2));

        // calculate row of i + 1 and put in row2
        blur_line(i, height, width, row2, image);

        //end of the function that blures a line.
        // having calculated the row2. paste the content of row 1 into image [i].
        memcpy(image[i], &row1, sizeof(row1));

    }


    return;
}


BYTE clamp( WORD color)
{
    if (color > 255) return 255;
    else return color;
}

void edge_line ( int i, int height, int width, RGBTRIPLE row2[width], RGBTRIPLE image[height][width])
{
    RGBTRIPLE var1;
    RGBTRIPLE var2;
    int blueX = 0, greenX = 0, redX = 0, blueY = 0, greenY = 0, redY = 0;




    char Gx, Gy = 0;
    for(int a = 1; a < 3; a++)
    {
        for(int b = 0; b < 3; b++)
        {
            if(i + b >= 0 && i + b < height && a < width ) // verify conditions of a.
            {

                // a is for width and b is for height.
                //calculate the pixel image[i][j+1]
                Gx = (2 - abs(b-1)) * (a - 1);
                Gy = (2 - abs(a-1)) * (b - 1);

                blueX += image[i + b][a -1].rgbtBlue * Gx;
                blueY += image[i + b][a -1].rgbtBlue * Gy;

                greenX += image[i+ b][a -1].rgbtGreen * Gx;
                greenY += image[i+ b][a -1].rgbtGreen * Gy;

                redX += image[i+ b][a -1].rgbtRed * Gx;
                redY += image[i+ b][a-1].rgbtRed * Gy;
            }
        }
    }


    var2.rgbtBlue = clamp(round(sqrt(pow(blueX,2) + pow(blueY,2))));
    var2.rgbtGreen = clamp(round(sqrt(pow(greenX,2) + pow(greenY,2))));
    var2.rgbtRed = clamp(round(sqrt(pow(redX,2) + pow(redY,2))));

    for(int j = 0; j < width ; j++)
    {

        var1 = var2;

        //Calculate the bloc of j + 1 and put it in var2


        blueX = 0;
        greenX = 0;
        redX = 0;

        blueY = 0;
        greenY = 0;
        redY = 0;

        for(int a = 0; a < 3; a++)
        {
            for(int b = 0; b < 3; b++)
            {
                if((i+b >= 0 && i+b < height) && (j + a >= 0 && j + a < width))
                {
                    // a is for width and b is for height.
                    //calculate the pixel image[i][j+1]
                    Gx = (2 - abs(b-1)) * (a-1);
                    Gy = (2 - abs(a-1)) * (b - 1);

                    blueX += image[i + b][a + j].rgbtBlue * Gx;
                    blueY += image[i + b][a + j].rgbtBlue * Gy;

                    greenX += image[i+ b][a + j].rgbtGreen * Gx;
                    greenY += image[i+ b][a + j].rgbtGreen * Gy;

                    redX += image[i+ b][a + j].rgbtRed * Gx;
                    redY += image[i+ b][a + j].rgbtRed * Gy;
                }
            }
        }


        var2.rgbtBlue = clamp(round(sqrt(pow(blueX,2) + pow(blueY,2))));
        var2.rgbtGreen = clamp(round(sqrt(pow(greenX,2) + pow(greenY,2))));
        var2.rgbtRed = clamp(round(sqrt(pow(redX,2) + pow(redY,2))));

        // image[i][j] = var1;
        row2[j] = var1;
    }
    return;
}
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // 1 itterate over the whole row computing the blured values for each pixel => store in array of type RGBTRIPLE pixel_1[width];

    //declare two rows of size width of type RGBTRIPLE.
    RGBTRIPLE row1[width], row2[width];


    edge_line(-1, height, width, row2, image);

    for(int i = 0; i < height; i++)
    {

        // get a copy of row2 in row 1 and proceed ot calculate row2 before actualy modifying the values of image[i] ith row 1 values;
        memcpy(&row1, &row2, sizeof(row2));

        // calculate row of i + 1 and put in row2
        edge_line(i, height, width, row2, image);

        //end of the function that blures a line.
        // having calculated the row2. paste the content of row 1 into image [i].
        memcpy(image[i], &row1, sizeof(row1));

    }
    return;
}
