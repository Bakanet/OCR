#ifndef LINE_H
#define LINE_H

int countBlackPixels(SDL_Surface *image, SDL_Surface *copy, int i, int j);
void drawHorizontalLine(SDL_Surface *image, SDL_Surface *copy, int i, int j);
void lineSegmentation(SDL_Surface *image, SDL_Surface *copy);

#endif
