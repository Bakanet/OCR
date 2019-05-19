#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix Matrix;
struct Matrix
{
    size_t height;
    size_t width;
    double *array;
};

//Using the Matrix
Matrix *initMatrix(size_t height, size_t width);
void freeMatrix(Matrix *matrix);
void printMatrix(Matrix *matrix);
void resize(Matrix *matrix, Matrix *answer, int size);
void intoSquare(Matrix *matrix, Matrix *answer);

//BasicOperations
void transpose(Matrix *m, Matrix *result);
void add(Matrix *m1, Matrix *m2, Matrix *result);
void mul(Matrix *m1, Matrix *m2, Matrix *result);
void scalMul(Matrix *matrix, double scal, Matrix *result);
void wise(Matrix *m1, Matrix *m2, Matrix *result);

//Operations needed by the Network
void sigmoid(Matrix *matrix);
void gradient(double eta, Matrix *error, Matrix *layer, Matrix *answer);

#endif
