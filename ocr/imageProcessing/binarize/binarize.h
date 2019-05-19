#ifndef BINARIZE_H
#define BINARIZE_H

//Main function
void binarize(SDL_Surface *image);

//GreyScale
void greyScale(SDL_Surface *surface);

//Sauvola's method
double meanSauvola(SDL_Surface *image, int w, int h, int mask);
double variance2(SDL_Surface *image, int w, int h, int mask);
double thresholdSauvole(SDL_Surface *image, int mask);
void blackAndWhiteSauvola(SDL_Surface *image, int mask);

#endif
