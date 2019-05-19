#ifndef SAVE_H
#define SAVE_H

int saveMatrix(Matrix *mat, char *path, char *mode);
Matrix *loadMatrix(char *path, size_t line);
size_t countLines(char *path);

#endif
