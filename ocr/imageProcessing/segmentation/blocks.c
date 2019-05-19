#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../sdlBasics.h"
#include "line.h"
#include "char.h"
#include "blocks.h"



/***************/
/*Main function*/
/***************/

//Main function that cut the blocks
void Segment (SDL_Surface *image, SDL_Surface *copy)
{
    extendHorizontally(image, copy);

    extendVertically(image, copy);

    deleteAllBlankBlocks(image, copy);

    improveBlocks(image, copy);

    SDL_SaveBMP(image, "FilesTreatement/imagesBlocks.bnp");
    SDL_SaveBMP(image, "FilesTreatement/copyBlocks.bnp");

    lineSegmentation(image, copy);
    SDL_SaveBMP(image, "FilesTreatement/imagesLines.bnp");
    SDL_SaveBMP(image, "FilesTreatement/copyLines.bnp");
    charSegmentation(image, copy);
    SDL_SaveBMP(image, "FilesTreatement/imagesChars.bnp");
    SDL_SaveBMP(image, "FilesTreatement/copyChars.bnp");
}




/*******************/
/*Create the blocks*/
/*******************/

//Extend characters horizontally (like in RLSA method)
void extendHorizontally(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 currentPixel;
    Uint32 blackPixel = SDL_MapRGB(copy->format, 0, 0, 0);

    for (int i = 0; i < image->h; ++i)
    {
        for (int j = 0; j < image->w; ++j)
        {
            currentPixel = getpixel(image, j, i);
            if (testWhite(currentPixel, image))
            {
                if (nextBlackPixelHorizontal(i, j, image) < image->w / 10)
                {
                    putpixel(copy, j, i, blackPixel);
                }
            }
           else if (j + image->w / 10 < image->w)
            {
                for (int k = 0; j < image->w && k < image->w / 10; k++)
                {
                    putpixel(copy, ++j, i, blackPixel);
                }
            }
        }
    }
}



//Return the index of the next black pixel horizontally
int nextBlackPixelHorizontal(int i, int j, SDL_Surface *image)
{
    int answer = 0;
    Uint32 pixel = getpixel(image, j, i);

    while (j < image->w - 1 && testWhite(pixel, image))
    {
        ++answer;
        pixel = getpixel(image, ++j, i);
    }

    if (j == image->w - 1)
        return image->w;
    return answer;
}




//Extend character vertically and apply a "and" operator (like in RLSA method)
void extendVertically(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 currentPixel;
    Uint32 blackPixel = SDL_MapRGB(copy->format, 0, 0, 0);
    Uint32 whitePixel = SDL_MapRGB(copy->format, 255, 255, 255);

    for (int j = 0; j < image->w; ++j)
    {
        for (int i = 0; i < image->h; ++i)
        {
            currentPixel = getpixel(image, j, i);
            if (testWhite(currentPixel, image))
            {
                currentPixel = getpixel(copy, j, i);
                if (nextBlackPixelVertical(i, j, image) < image->h / 10 &&
                        testBlack(currentPixel, copy))
                    putpixel(copy, j, i, blackPixel);
                else
                    putpixel(copy, j, i, whitePixel);
            }
            else if (i + image->h / 10 < image->h)
            {
                for (int k = 0; i < image->h && k < image->h / 10; k++)
                {
                    currentPixel = getpixel(copy, j, ++i);
                    if (testWhite(currentPixel, copy))
                        putpixel(copy, j, i, whitePixel);
                }
            }
        }
    }
}



//Return the index of the next bleck pixel Vertically
int nextBlackPixelVertical(int i, int j, SDL_Surface *image)
{
    int answer = 0;
    Uint32 pixel = getpixel(image, j, i);

    while (i < image->h - 1 && testWhite(pixel, image))
    {
        ++answer;
        pixel = getpixel(image, j, ++i);
    }

    if (i == image->h - 1)
        return image->h;
    return answer;
}







/****************************/
/*Removing not wanted blocks*/
/****************************/

//Delete all blank blocks
void deleteAllBlankBlocks(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 pixel;

    for (int i = 0; i < copy->h; ++i)
    {
        for (int j = 0; j < copy->w; ++j)
        {
            pixel = getpixel(copy, j, i);
            if (testBlack(pixel, copy))
            {
                int k = studyBlock(image, copy, i, j, 0);
                if (!k)
                {
                    k = 2;
                    deleteBlank(copy, i, j);
                }
            }
        }
    }
}



//Delete the block that we want
void deleteBlank(SDL_Surface *copy, int i, int j)
{
    Uint32 whitePixel = SDL_MapRGB(copy->format, 255, 255, 255);
    Uint32 pixel = getpixel(copy, j, i);

    if (testGreen(pixel, copy))
    {
        putpixel(copy, j, i, whitePixel);
        if (i < copy->h - 1)
            deleteBlank(copy, i + 1, j);

        if (i > 0)
            deleteBlank(copy, i - 1, j);

        if (j < copy->w - 1)
            deleteBlank(copy, i, j + 1);

        if (j > 0)
            deleteBlank(copy, i, j - 1);
    }
}



//Return if there is a character under the block
//(and color the image in green to see what pixel have already been treated)
int studyBlock(SDL_Surface *image, SDL_Surface *copy, int i, int j, int black)
{
    Uint32 pixel = getpixel(image, j, i);
    Uint32 copyPixel = getpixel(copy, j, i);
    Uint32 greenPixel = SDL_MapRGB(copy->format, 0, 255, 0);

    if (testWhite(copyPixel, copy))
        return black;

    else
    {
        putpixel(copy, j, i, greenPixel);
        if (testBlack(pixel, image))
            black = 1;

        if (testBlack(copyPixel, copy))
        {
            if (i < image->h - 1)
            {
                if (studyBlock(image, copy, i + 1, j, black))
                    black = 1;
            }

            if (i > 0)
            {
                if (studyBlock(image, copy, i - 1, j, black))
                    black = 1;
            }

            if (j < image->w - 1)
            {
                if (studyBlock(image, copy, i, j + 1, black))
                    black = 1;
            }

            if (j > 0)
            {
                if (studyBlock(image, copy, i, j - 1, black))
                    black = 1;
            }
        }
    }

    return black;
}





/***************************/
/*Making blocks look better*/
/***************************/

//Compute all the functions useful to make blocks look better
void improveBlocks(SDL_Surface *image, SDL_Surface *copy)
{
    int changes = 1;

    reduceLineSpaces(copy);
    while (changes)
    {
        changes = stickBlocksHorizontally(copy) ||
            stickBlocksVertically(copy);
    }
    cutBlocks(image, copy);
}



//Delete linespaces to stick the paragraphs together (big value for interspace
//chosen after many tests to be the most appropriate)
//(and recolor the blocks in black)
void reduceLineSpaces(SDL_Surface *copy)
{
    int nbBlackPixels;
    int nbWhitePixels;
    Uint32 pixel;
    Uint32 blackPixel = SDL_MapRGB(copy->format, 0, 0, 0);

    for (int j = 0; j < copy->w; ++j)
    {
        for (int i = 0; i < copy->h; ++i)
        {
            nbBlackPixels = 0;
            nbWhitePixels = 0;
            pixel = getpixel(copy, j, i);
            while (i < copy->h - 1 && testGreen(pixel, copy))
            {
                nbBlackPixels++;
                putpixel(copy, j, i, blackPixel);
                pixel = getpixel(copy, j, ++i);
            }
            int indexReach = i;
            while (i < copy->h - 1 && testWhite(pixel, copy))
            {
                nbWhitePixels++;
                pixel = getpixel(copy, j, ++i);
            }
            if (nbBlackPixels * 5 / 4 > nbWhitePixels && i != copy->h - 1)
            {
                for (int k = indexReach; k < i; ++k)
                {
                    putpixel(copy, j, k, blackPixel);
                }
            }
            if (i < copy->h - 1)
                --i;
        }
    }
}



//Stick blocks that are really close one from another
//the threshold is taken arbitrarily after many tests
//return 1 if there was a change, else return 0
int stickBlocksHorizontally(SDL_Surface *copy)
{
    int changes = 0;
    int nbWhitePixels;
    Uint32 pixel;
    Uint32 blackPixel = SDL_MapRGB(copy->format, 0, 0, 0);

    for (int j = 0; j < copy->w; ++j)
    {
        for (int i = 0; i < copy->h; ++i)
        {
            nbWhitePixels = 0;
            pixel = getpixel(copy, j, i);
            while (i < copy->h - 1 && testBlack(pixel, copy))
            {
                pixel = getpixel(copy, j, ++i);
            }
            int indexReach = i;
            while (i < copy->h - 1 && testWhite(pixel, copy))
            {
                nbWhitePixels++;
                pixel = getpixel(copy, j, ++i);
            }
            if (nbWhitePixels < copy->h / 60 && i != copy->h - 1 &&
                    indexReach != 0)
            {
                changes = 1;
                for (int k = indexReach; k <= i; ++k)
                {
                    putpixel(copy, j, k, blackPixel);
                }
            }
            if (i < copy->h - 1)
                --i;
        }
    }
    return changes;
}



//Stick blocks that are really close one from another
//the threshold is taken arbitrarily after many tests
//return 1 if there was a change, else return 0
int stickBlocksVertically(SDL_Surface *copy)
{
    int changes = 0;
    int nbWhitePixels;
    Uint32 pixel;
    Uint32 blackPixel = SDL_MapRGB(copy->format, 0, 0, 0);

    for (int i = 0; i < copy->h; ++i)
    {
        for (int j = 0; j < copy->w; ++j)
        {
            nbWhitePixels = 0;
            pixel = getpixel(copy, j, i);
            while (j < copy->w - 1 && testBlack(pixel, copy))
            {
                pixel = getpixel(copy, ++j, i);
            }
            int indexReach = j;
            while (j < copy->w - 1 && testWhite(pixel, copy))
            {
                nbWhitePixels++;
                pixel = getpixel(copy, ++j, i);
            }
            if (nbWhitePixels < copy->w / 60 && j != copy->w - 1 &&
                    indexReach != 0)
            {
                changes = 1;
                for (int k = indexReach; k < j; ++k)
                {
                    putpixel(copy, k, i, blackPixel);
                }
            }
            if (j < copy->h - 1)
                --j;
        }
    }
    return changes;
}



//Put red pixels all around the blocks on both original and copy images
void cutBlocks(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 pixel;
    Uint32 copyRedPixel = SDL_MapRGB(copy->format, 255, 0, 0);
    Uint32 originalRedPixel = SDL_MapRGB(image->format, 255, 0, 0);

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            pixel = getpixel(copy, j, i);
            if (testWhite(pixel, copy))
            {
                if (i < image->h - 1)
                {
                    pixel = getpixel(copy, j, i + 1);
                    if (testBlack(pixel, copy))
                    {
                        putpixel(copy, j, i, copyRedPixel);
                        putpixel(image, j, i, originalRedPixel);
                        continue;
                    }
                }

                if (i > 0)
                {
                    pixel = getpixel(copy, j, i - 1);
                    if (testBlack(pixel, copy))
                    {
                        putpixel(copy, j, i, copyRedPixel);
                        putpixel(image, j, i, originalRedPixel);
                        continue;
                    }
                }

                if (j < image->w - 1)
                {
                    pixel = getpixel(copy, j + 1, i);
                    if (testBlack(pixel, copy))
                    {
                        putpixel(copy, j, i, copyRedPixel);
                        putpixel(image, j, i, originalRedPixel);
                        continue;
                    }
                }

                if (j > 0)
                {
                    pixel = getpixel(copy, j - 1, i);
                    if (testBlack(pixel, copy))
                    {
                        putpixel(copy, j, i, copyRedPixel);
                        putpixel(image, j, i, originalRedPixel);
                    }
                }
            }
        }
    }
}
