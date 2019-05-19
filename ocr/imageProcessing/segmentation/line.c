#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "../sdlBasics.h"
#include "line.h"



//Count the number of black pixels on a line
int countBlackPixels(SDL_Surface *image, SDL_Surface *copy, int i, int j)
{
    Uint32 imagePixel = getpixel(image, j, i);
    Uint32 copyPixel = getpixel(copy, j, i);

    int nbBlackPixels = 0;

    while (j < image->w && !testRed(copyPixel, copy))
    {
        if(testBlack(imagePixel, image))
        {
            nbBlackPixels++;
        }
        j++;
        copyPixel = getpixel(copy, j, i);
        imagePixel = getpixel(image, j, i);
    }
    return nbBlackPixels;
}



//Draw a horizontal green line on the block
void drawHorizontalLine(SDL_Surface *image, SDL_Surface *copy, int i, int j)
{
    Uint32 blue = SDL_MapRGB(image->format, 0, 0, 255);
    Uint32 copyPixel = getpixel(copy, j, i);
    Uint32 imagePixel = getpixel(image, j, i);

    while(j < image->w && !testRed(copyPixel, copy))
    {
        if(!testBlack(imagePixel, image))
        {
            putpixel(image, j, i, blue);
        }
        j++;
        copyPixel = getpixel(copy, j, i);
        imagePixel = getpixel(image, j, i);
    }
}



//Separate each line in a block
void lineSegmentation(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 pixel;
    int nbBlackPixels;

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            pixel = getpixel(copy, j, i);
            if (testRed(pixel, copy))
            {
                if (j < image->w - 1 &&
                        testBlack(getpixel(copy, j + 1, i), copy))
                {
                    nbBlackPixels = countBlackPixels(image, copy, i, j + 1);
                    if (nbBlackPixels == 0)
                    {
                        drawHorizontalLine(image, copy, i, j + 1);
                    }
                }
            }
        }
    }
}
