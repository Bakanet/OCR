#ifndef SEGMENTATION_H
#define SEGMENTATION_H

//Main function
void Segment(SDL_Surface *image, SDL_Surface *copy);

//Cutting blocks
void extendHorizontally(SDL_Surface *image, SDL_Surface *copy);
int nextBlackPixelHorizontal(int i, int j, SDL_Surface *image);
void extendVertically(SDL_Surface *image, SDL_Surface *copy);
int nextBlackPixelVertical(int i, int j, SDL_Surface *image);

//Removing non wanted blocks
void deleteAllBlankBlocks(SDL_Surface *image, SDL_Surface *copy);
int studyBlock(SDL_Surface *image, SDL_Surface *copy, int i, int j, int black);
void deleteBlank(SDL_Surface *copy, int i, int j);

//Making blocks look better
void improveBlocks(SDL_Surface *image, SDL_Surface *copy);
void reduceLineSpaces(SDL_Surface *copy);
int stickBlocksHorizontally(SDL_Surface *copy);
int stickBlocksVertically(SDL_Surface *copy);
void cutBlocks(SDL_Surface *image, SDL_Surface *copyImage);

#endif
