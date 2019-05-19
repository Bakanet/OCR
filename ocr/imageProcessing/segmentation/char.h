#ifndef CHAR_H
#define CHAR_H

int colorBelow(SDL_Surface *image, int i, int j);
void drawVerticalLine(SDL_Surface *image, int i, int j, int nextColorindex);
void charSegmentation(SDL_Surface *image, SDL_Surface *copy);

#endif
