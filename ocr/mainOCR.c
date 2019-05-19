#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "imageProcessing/sdlBasics.h"
#include "imageProcessing/binarize/binarize.h"
#include "imageProcessing/segmentation/blocks.h"
#include "NeuralNetwork/matrix.h"
#include "NeuralNetwork/network.h"
#include "FilesTreatement/save.h"
#include "FilesTreatement/makeInputs.h"
#include "imageProcessing/parsing/parsing.h"

void launchOCR()
{
    initSDL();
    SDL_Surface *image = loadImage("images/txt.png");
    binarize(image);
    SDL_Surface *copy = newSurface(image->w, image->h);
    SDL_BlitSurface(image, NULL, copy, NULL);
    Segment(image, copy);
    parseImage(image, copy);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(image);
    SDL_Quit();
}



void trainNetwork()
{
    size_t *layers = calloc(3, sizeof(size_t));
    layers[0] = 28*28;
    layers[1] = 95;
    layers[2] = 94;
    Network *net = initNetwork(layers);
    train(net, 100000, .03, "FilesTreatement/input", "FilesTreatement/output");
    freeNetwork(net);
}
