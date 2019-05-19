#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "sdlBasics.h"




/*********************************************/
/*Functions taking care of SDL library itself*/
/*********************************************/



//Init the SDL library, take care of the errors
int initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        errx(1, "SDL could not be initialised");
    }
    return 0;
}


//Freeze the executable until a key is pressed
void waitForKeypressed()
{
    SDL_Event event;
    for(;;)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN: return;
            default: break;
        }
    }
}



//Load the image, giving back a SDL_Surface, take care of errors
SDL_Surface *loadImage(char *path)
{
    SDL_Surface *image;
    image = IMG_Load(path);
    if (image == NULL)
        fprintf(stderr, "can't load image : %s", SDL_GetError());
    return image;
}



//Display the image on the screen durnig time seconds
void displayImage(SDL_Surface *image, Uint32 time)
{
    SDL_Window *screen;
    screen = SDL_CreateWindow("Main window", SDL_WINDOWPOS_UNDEFINED,
       SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
    if (screen == NULL)
        fprintf(stderr, "can't create window : %s", SDL_GetError());
    if(SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(screen), NULL) < 0)
        warnx("BlitSurface error");
    SDL_UpdateWindowSurface(screen);
    if (time == 666)
        waitForKeypressed();
    SDL_Delay(time);
    SDL_DestroyWindow(screen);
}






/****************************/
/*testing some pixels values*/
/****************************/



//Test if the pixel is black
int testBlack(Uint32 pixel, SDL_Surface *surface)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 0 && g == 0 && b == 0);
}



//Test if the pixel is red
int testRed(Uint32 pixel, SDL_Surface *surface)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 255 && g == 0 && b == 0);
}



//Test if the pixel is white
int testWhite(Uint32 pixel, SDL_Surface *surface)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 255 && g == 255 && b == 255);
}



//Test if the pixel is green
int testGreen(Uint32 pixel, SDL_Surface *surface)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 0 && g == 255 && b == 0);
}


//Test if the pixel is blue
int testBlue(Uint32 pixel, SDL_Surface *surface)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 0 && g == 0 && b == 255);
}





/******************************************************************/
/*All the following functions are taken from the SDL documentation*/
/******************************************************************/



//Create an empty surface
SDL_Surface *newSurface(int width, int height)
{
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    }
    else
    {
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                   rmask, gmask, bmask, amask);
    if(surface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    return surface;
}



//Return the value of the pixel at (x, y) coordinates
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;
    }
}



//Change the Value of the pixel at (x, y) coordinates
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
