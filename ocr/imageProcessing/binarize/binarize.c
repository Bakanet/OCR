#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdlBasics.h"
#include "binarize.h"




/**********************/
/*Binarizing the image*/
/**********************/



//Compute the greyscale and the complete Otsu's algorithm.
//Transform the image from colored to black and white
void binarize(SDL_Surface *image)
{
    greyScale(image);
    blackAndWhiteSauvola(image, 5);
}







/************************/
/*Greyscale of the image*/
/************************/



//Transform the image into a greyscale image, by takimg the average of
//green, red and blue value for each pixel
void greyScale(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;
    Uint32 average;
    Uint32 pixel;
    Uint8 r, g, b;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = getpixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            average = (r + g + b) / 3;
            pixel = SDL_MapRGB(image->format, average, average, average);
            putpixel(image, j, i, pixel);
        }
    }
}





/***************************************/
/*Sauvola's Method to binarize an image*/
/***************************************/

//return an average of the values of the pixel at a distance mask
//of the pixel treated
double meanSauvola(SDL_Surface *image, int w, int h, int mask)
{
    double mean = 0;
    int nbPixels = 0;
    Uint32 pixel;
    Uint8 r, g, b;

    for (double i = h - mask/2; i <= (h + mask/2); i++)
    {
        if (i >= 0 && i < image->h)
        {
            for (double j = w - mask/2; j <= (w + mask/2); j++)
            {
                if (j >= 0 && j < image->w)
                {
                    pixel = getpixel(image, j, i);
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);
                    mean += r;
                    nbPixels++;
                }
            }
        }
    }
    return mean/nbPixels;
}




//Calculate the variance of the pixel and the mask
double variance2(SDL_Surface *image, int w, int h, int mask)
{
    double mean = meanSauvola(image, w, h, mask);
    int nbPixels = 0;
    Uint32 pixel;
    Uint8 r, g, b;
    double pixelGrey;
    double variance2 = 0;

    for (double i = h - mask/2; i <= (h + mask/2); i++)
    {
        if (i >= 0 && i < image->h)
        {
            for (double j = w - mask/2; j <= (w + mask/2); j++)
            {
                if (j >= 0 && j < image->w)
                {
                    pixel = getpixel(image, j, i);
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);
                    pixelGrey = r;
                    variance2 += (pixelGrey - mean)*(pixelGrey - mean);
                    nbPixels++;
                }
            }
        }
    }
    return variance2/nbPixels;
}




//Calculate the Sauvola's threshold
double thresholdSauvola(SDL_Surface *image, int w, int h, int mask)
{
    double mean = meanSauvola(image, w, h, mask);
    double s = sqrt(variance2(image, w, h, mask));
    double threshold = mean * (1 + 0.5*((s/128) - 1));
    return threshold;
}




//Change the image into a black and white image
void blackAndWhiteSauvola(SDL_Surface *image, int mask)
{
    int width = image->w;
    int height = image->h;
    double threshold;
    Uint32 pixel;
    Uint32 blackPixel = SDL_MapRGB(image->format, 0, 0, 0);
    Uint32 whitePixel = SDL_MapRGB(image->format, 255, 255, 255);
    Uint8 r, g, b;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            threshold = thresholdSauvola(image, j, i, mask);
            pixel = getpixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r < threshold)
                putpixel(image, j, i, blackPixel);
            else
                putpixel(image, j, i, whitePixel);
        }
    }
}
