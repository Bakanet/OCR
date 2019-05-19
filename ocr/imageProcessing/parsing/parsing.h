#ifndef PARSING_H
#define PARSING_H

//Parsing
void parseImage(SDL_Surface *image, SDL_Surface *copy);
void parseBlock(SDL_Surface *image, SDL_Surface *copy, int i, int j);
void colorBlock(SDL_Surface *copy, int i, int j);
void parseLine(SDL_Surface *image, SDL_Surface *copy, int i, int j);
Matrix *parseChar(SDL_Surface *image, int i, int j);

//Finding the size of a character
int charHeight(SDL_Surface *image, int i, int j);
int charWidth(SDL_Surface *image, int i, int j);

//Finding the linsespace and the size of a space
int findAverageLinespace(SDL_Surface *image, SDL_Surface *copy);
int findAverageCharacterSpace(SDL_Surface *image, SDL_Surface *copy, int line);

#endif
