#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "../sdlBasics.h"
#include "char.h"



//Return the color of the next piexel below that is not white.
//It will be black if it's a character, and red if it's a new line.
int colorBelow(SDL_Surface *image, int i, int j)
{
    Uint32 pixel = getpixel(image, j, i);

    while(i < image->h - 1 && testWhite(pixel, image))
    {
        i++;
        pixel = getpixel(image, j, i);
    }
    return i;
}



//Draw a vertical line from i to nextColorIndex value
void drawVerticalLine(SDL_Surface *image, int i, int j, int nextColorIndex)
{
    Uint32 red = SDL_MapRGB(image->format, 255, 0, 0);
    while (i <= nextColorIndex)
    {
        putpixel(image, j, i, red);
        i++;
    }
}



//Separate each character on the image
void charSegmentation(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 pixel;
    int nextColorIndex;
    int nextColorPixel;

    for (int j = 0; j < image->w; j++)
    {
        for (int i = 0; i < image->h; i++)
        {
            pixel = getpixel(image, j, i);
            if ((testBlue(pixel, image) || testRed(pixel, image))
                    && i < copy->h - 1)
            {
                i++;
                pixel =  getpixel(copy, j, i);
                Uint32 pixel2 = getpixel(image, j, i);
                if (testBlack(pixel, copy) && !testBlue(pixel2, image)
                        && !testRed(pixel2, image))
                {
                    nextColorIndex = colorBelow(image, i, j);
                    nextColorPixel = getpixel(image, j, nextColorIndex);
                    if (testBlue(nextColorPixel, image) ||
                            testRed(nextColorPixel, image))
                        drawVerticalLine(image, i, j, nextColorIndex);
                    else
                        i = nextColorIndex;
                }
                else
                    --i;
            }
        }
    }
}
