#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../imageProcessing/sdlBasics.h"
#include "../imageProcessing/binarize/binarize.h"
#include "../NeuralNetwork/matrix.h"
#include "../NeuralNetwork/network.h"
#include "save.h"

//Create the inputs file
void makeInput(char argv[])
{
    initSDL();
    SDL_Surface *img = loadImage(argv);
    if (img == NULL)
        errx(1, "path not found");
    Matrix *matrix = initMatrix(img->h, img->w);
    Uint32 pixel;
    for (int i = 0; i < img->h; i++)
    {
        for (int j = 0; j < img->w; j++)
        {
            pixel = getpixel(img, j, i);
            matrix->array[i * img->w + j] = testWhite(pixel, img);
        }
    }
    size_t size = 28;
    Matrix *resizedMatrix = initMatrix(size * size, 1);
    resize(matrix, resizedMatrix, size);

    saveMatrix(resizedMatrix, "./input", "a");
    SDL_Quit();
}



//Create the outputs file
void makeOutput(char argv[])
{
    Matrix *matrix = initMatrix(94, 1);
    size_t answer = 0;
    size_t index = 0;
    while (argv[index] < '0' || argv[index] > '9')
        index++;
    while (argv[index] >= '0' && argv[index] <= '9')
    {
        answer *= 10;
        answer += (argv[index] - '0');
        index++;
    }
    matrix->array[answer - 33] = 1;
    saveMatrix(matrix, "./output", "a");
}
