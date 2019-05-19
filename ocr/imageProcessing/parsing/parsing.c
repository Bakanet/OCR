#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include "../sdlBasics.h"
#include "../../NeuralNetwork/matrix.h"
#include "../../NeuralNetwork/network.h"
#include "parsing.h"



/*********/
/*Parsing*/
/*********/

//Parse the entire image
void parseImage(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 copyPixel = getpixel(copy, 0, 0);
    FILE *file = fopen("FilesTreatement/textOCR", "w");
                printf("   ");
    fclose(file);

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            copyPixel = getpixel(copy, j, i);
            if (!testWhite(copyPixel, copy))
            {
                parseBlock(image, copy, i, j);
            }
        }
    }
}




//Parse a block of text
void parseBlock(SDL_Surface *image, SDL_Surface *copy, int i, int j)
{
    Uint32 imagePixel = getpixel(image, j, i);
    Uint32 copyPixel = getpixel(copy, j, i);
    Uint32 whitePixel = SDL_MapRGB(copy->format, 255, 255, 255);
    Uint32 tmpPixel = getpixel(image, j, i);
    int x, y;

    colorBlock(copy, i, j);

    int averageLineSpace = findAverageLinespace(image, copy);

    for (int i = 0; i < copy->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            copyPixel = getpixel(copy, j, i);
            if (testGreen(copyPixel, copy))
            {
                imagePixel = getpixel(image, j, i);
                if (!testGreen(imagePixel, image) && (testWhite(imagePixel, image)
                        || testBlack(imagePixel, image)))
                {
                    x = j;
                    y = i;
                    if (y + averageLineSpace < image->h)
                    {
                        y += averageLineSpace / 2;
                    }
                    tmpPixel = getpixel(copy, x, y);
                    while(x > 0 && testGreen(tmpPixel, copy))
                    {
                        tmpPixel = getpixel(copy, --x, y);
                    }
                    parseLine(image, copy, y, x);
                }
                putpixel(copy, j, i, whitePixel);
            }
        }
    }
}




//Color a block of text in green in order to make it different from the others
void colorBlock(SDL_Surface *copy, int i, int j)
{
    Uint32 greenPixel = SDL_MapRGB(copy->format, 0, 255, 0);
    Uint32 copyPixel = getpixel(copy, j, i);

    if (testRed(copyPixel, copy) || testBlack(copyPixel, copy))
    {
        putpixel(copy, j, i, greenPixel);
        if (i < copy->h - 1)
            colorBlock(copy, i + 1, j);
        if (i > 0)
            colorBlock(copy, i - 1, j);
        if (j < copy->w - 1)
            colorBlock(copy, i, j + 1);
        if (j > 0)
            colorBlock(copy, i, j - 1);
    }
}




//Parse a line of text, taking care of the spaces characters
void parseLine(SDL_Surface *image, SDL_Surface *copy, int i, int j)
{
    Uint32 imagePixel = getpixel(image, ++j, i);
    Uint32 copyPixel = getpixel(copy, j, i);
    Matrix *character = NULL;
    int averageSpace = findAverageCharacterSpace(image, copy, i);
    int nbSpacesStraight = 0;
    int NextChar = 0;
    int isAChar = 0;
    FILE *file = fopen("FilesTreatement/textOCR", "a");
    Network *network = initNetwork(NULL);
    char answer;

    while (j < image->w - 1 && testGreen(copyPixel, copy))
    {
        imagePixel = getpixel(image, j, i);
        if (nbSpacesStraight == 0 && (NextChar == j || NextChar == 0) && 
                testGreen(copyPixel, copy)) 
        {
            int k = 0;
            while (j + k < image->w - 1 && (testRed(imagePixel, image)) && 
                    testGreen(copyPixel, copy))
            {
                nbSpacesStraight++;
                k++;
                imagePixel = getpixel(image, j + k, i);
                copyPixel = getpixel(copy, j + k, i);
            }
            NextChar = j + k + 1;
        }

        imagePixel = getpixel(image, j, i);
        if ((testWhite(imagePixel, image) || testBlack(imagePixel, image)) && 
                !testGreen(imagePixel, image))
        {
            if (nbSpacesStraight > averageSpace)
            {
                fprintf(file, " ");
            }
            nbSpacesStraight = 0;
            int size = 28;
            Matrix *resizedChar = initMatrix(size * size, 1);
            character = parseChar(image, i, j);
            resize(character, resizedChar, size);
            network->input = resizedChar;
            answer = detectCharacter(network);
            putc(answer, file);

            freeMatrix(character);
            freeMatrix(resizedChar);
            isAChar = 1;
        }
        
        copyPixel = getpixel(copy, ++j, i);
    }
    if (isAChar)
    {
        fprintf(file, "\n");
    }
    fclose(file);
    freeNetwork(network);
}




//Parse a character and color it in red
Matrix *parseChar(SDL_Surface *image, int i, int j)
{
    Uint32 imagePixel = getpixel(image, j, i);
    Uint32 green = SDL_MapRGB(image->format, 0, 255, 0);

    while (i > 0 && !testBlue(imagePixel, image) && !testRed(imagePixel, image))
        imagePixel = getpixel(image, j, --i);

    imagePixel = getpixel(image, j, ++i);

    while (j > 0 && !testBlue(imagePixel, image) && !testRed(imagePixel, image))
        imagePixel = getpixel(image, --j, i);

    int beginLine = ++j;
    int width = charWidth(image, i, j);
    int height = charHeight(image, i, j);
    int x = 0;
    int y = 0;
    Matrix *character = initMatrix(height, width);

    while (x < height)
    {
        while (y < width)
        {
            imagePixel = getpixel(image, j, i);
            character->array[x * width + y++] = testWhite(imagePixel, image);
            putpixel(image, j++, i, green);
        }
        x++;
        i++;
        j = beginLine;
        y = 0;
    }

    return character;
}




/*********************************/
/*Finding the size of a character*/
/*********************************/

//return the height of the character located in the i, j position
int charHeight(SDL_Surface *image, int i, int j)
{
    Uint32 imagePixel = getpixel(image, j, i);
    int height = 0;

    while (i < image->h - 1 && !testRed(imagePixel, image) &&
            !testBlue(imagePixel, image))
    {
        height++;
        imagePixel = getpixel(image, j, ++i);
    }
    return height;
}




//return the width of the character located in the i, j position
int charWidth(SDL_Surface *image, int i, int j)
{
    Uint32 imagePixel = getpixel(image, j, i);
    int width = 0;

    while (j < image->w - 1 && !testRed(imagePixel, image) &&
            !testBlue(imagePixel, image))
    {
        width++;
        imagePixel = getpixel(image, ++j, i);
    }
    return width;
}




/******************/
/*Calculate spaces*/
/*****************/

//Calculate the average of a space between each character of a line in order to
//deduce a possible value for the size of space character
int findAverageCharacterSpace(SDL_Surface *image, SDL_Surface *copy, int line)
{
    Uint32 copyPixel;
    Uint32 imagePixel;
    int nbPixels  = 0;
    int nbSpaces = 0;
    int onInterChar = 1;

    for (int j = 0; j < image->w; j++)
    {
        copyPixel = getpixel(copy, j, line);
        if (testGreen(copyPixel, copy))
        {
            imagePixel = getpixel(image, j, line);
            if (testRed(imagePixel, image))
            {
                nbPixels++;
                if (!onInterChar)
                {
                    nbSpaces++;
                    onInterChar = 1;
                }
            }
            else
            {
                onInterChar = 0;
            }
        }
    }

    if (nbSpaces == 0)
        return 0;
    return nbPixels / nbSpaces;
}




//Calculate the average of the linespace in a block of text
int findAverageLinespace(SDL_Surface *image, SDL_Surface *copy)
{
    Uint32 copyPixel;
    Uint32 imagePixel;
    int totalPixels = 0;
    int totalLines = 1;
    int nbPixels, nbLines, onInterline;

    for (int j = 0; j < image->w; j += image->w / 10)
    {
        nbPixels = 0;
        nbLines = 1;
        onInterline = 0;

        for (int i = 0; i < image->h; i++)
        {
            copyPixel = getpixel(copy, j, i);
            if (testGreen(copyPixel, copy))
            {
                nbPixels++;
                imagePixel = getpixel(image, j, i);
                if (testBlue(imagePixel, image))
                {
                    if (!onInterline)
                    {
                        nbLines++;
                        onInterline = 1;
                    }
                }
                else
                {
                    onInterline = 0;
                }
            }
        }
        if (nbLines != 1)
            totalLines += nbLines;
        totalPixels += nbPixels;
    }

    return totalPixels / totalLines;
}
