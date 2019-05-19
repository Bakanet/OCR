#ifndef SDLBASICS_H
#define SDLBASICS_H

//SDL library
int initSDL();
void waitForKeypressed();
SDL_Surface *loadImage(char *path);
void displayImage(SDL_Surface *image, Uint32 time);

//testing pixel values
int testBlack(Uint32 pixel, SDL_Surface *image);
int testRed(Uint32 pixel, SDL_Surface *image);
int testWhite(Uint32 pixel, SDL_Surface *image);
int testGreen(Uint32 pixel, SDL_Surface *image);
int testBlue(Uint32 pixel, SDL_Surface *image);

//Functions from SDL library
Uint32 getpixel(SDL_Surface *image, int x, int y);
void putpixel(SDL_Surface *image, int x, int y, Uint32 pixel);
SDL_Surface *newSurface(int width, int height);

#endif
