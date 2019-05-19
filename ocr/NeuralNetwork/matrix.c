#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"



/******************/
/*Using the matrix*/
/******************/

//Init a matrix of height and width given in paraneter
Matrix *initMatrix(size_t height, size_t width)
{
    Matrix *matrix = malloc(sizeof(*matrix));
    if (matrix == NULL)
        errx(1, "malloc fail");
    matrix->height = height;
    matrix->width = width;
    matrix->array = calloc(height * width, sizeof(double));
    if (matrix->array == NULL)
        errx(1, "malloc failopop");
    return matrix;
}




//Free the array of the Matrix then free the Matrix itself
void freeMatrix(Matrix *matrix)
{
    free(matrix->array);
    free(matrix);
}




//Print the array of the Matrix
void printMatrix(Matrix *matrix)
{
    for (size_t i = 0; i < matrix->height; i++)
    {
        for (size_t j = 0; j < matrix->width; j++)
        {
            printf("%.2f", matrix->array[i * matrix->width + j]);
        }
        printf("\n");
    }
    printf("\n");
}




//Resize the matrix into the size given
void resize(Matrix *matrix, Matrix *answer, int size)
{
    int maximum = (matrix->width > matrix->height) ?
        matrix->width : matrix->height;
    Matrix *squareMatrix = initMatrix(maximum, maximum);
    for (int i = 0; i < maximum; i++)
    {
        for (int j = 0; j < maximum; j++)
        {
            squareMatrix->array[i * maximum + j] = 1;
        }
    }
    intoSquare(matrix, squareMatrix);
    double ratio = (double) squareMatrix->height / (double) (size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int index = (int) ((double) i * ratio) 
                * maximum + ((double) j * ratio);
            double value = squareMatrix->array[index];
            answer->array[i * size + j] = value;
        }
    }
    freeMatrix(squareMatrix);
}




//if the matrix is not a suare matrix, convert it into a saure matrix
void intoSquare(Matrix *matrix, Matrix *answer)
{
    int width = matrix->width;
    int height = matrix->height;
    if (height < width)
    {
        int missingLines = (width - height) / 2;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                answer->array[(i + missingLines) * width + j]
                    = matrix->array[i * width + j];
            }
        }
    }
    else
    {
        int missingCols = (height - width) / 2;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                answer->array[i * width + (j + missingCols)]
                    = matrix->array[i * width + j];
            }
        }
    }
}





/******************/
/*Basic Operations*/
/******************/

//Transpose the Matrix into the result given in parameters
void transpose(Matrix *m, Matrix *result)
{
    if (m == result)
        errx(1, "It is not allowed to transpose yourself");
    if (m->width != result->height || m->height != result->width)
        errx(2, "Result matrix is not from the right size");

    for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->width; j++)
        {
            result->array[j * m->height + i] = m->array[i * m->width + j];
        }
    }
}




//Add two Matrix of the same size
void add(Matrix *m1, Matrix *m2, Matrix *result)
{
    if (m1->height != m2->height || m1->width != m2->width)
        errx(1, "Matrix can't be added : wrong size");
    if (result->height != m1->height || result->width != m1->width)
        errx(2, "Result matrix is not from the right size");

    size_t height = m1->height;
    size_t width = m1->width;

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            result->array[i * width + j] =
                m1->array[i * width + j] + m2->array[i * width + j];
        }
    }
}




//Multiply the elements of two matrix of the same size one by one
void wise(Matrix *m1, Matrix *m2, Matrix *result)
{
    if (m1->height != m2->height || m1->width != m2->width)
        errx(1, "Matrix can't be added : wrong size");
    if (result->height != m1->height || result->width != m1->width)
        errx(2, "Result matrix is not from the right size");

    size_t height = m1->height;
    size_t width = m1->width;

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            result->array[i * width + j] =
                m1->array[i * width + j] * m2->array[i * width + j];
        }
    }
}




//Multiply two Matrix
void mul(Matrix *m1, Matrix *m2, Matrix *result)
{
    if (m1->width != m2->height)
        errx(1, "Matrix can't be added : wrong size");
    if (m1->height != result->height || m2->width != result->width)
        errx(2, "Result matrix is not from the right size");


    size_t height = m1->height;
    size_t width = m2->width;

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            for (size_t k = 0; k < m1->width; k++)
            {
                result->array[i * width + j] +=
                    m1->array[i * m1->width + k] * m2->array[k * width + j];
            }
        }
    }
}




//Multiply a Matrix by a scalar
void scalMul(Matrix *matrix, double scal, Matrix *result)
{
    if (result->height != matrix->height || result->width != matrix->width)
        errx(1, "Result matrix is not from the right size");

    size_t height = matrix->height;
    size_t width = matrix->width;

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            result->array[i * width + j] *= scal;
        }
    }
}




/**********************************/
/*Operations needed by the network*/
/**********************************/

//Apply the sigmoid function on each element of the matrix
void sigmoid(Matrix *matrix)
{
    for (size_t i = 0; i < matrix->height; i++)
    {
        for (size_t j = 0; j < matrix->width; j++)
        {
            double value = 1 / (1 + exp(matrix->array[i * matrix->width + j]));
            matrix->array[i * matrix->width + j] = value;
        }
    }
}




//Apply the gradient on the two Matrix error and layer, with the scalar eta
void gradient(double eta, Matrix *error, Matrix *layer, Matrix *answer)
{
      wise(layer, layer, answer);
      scalMul(answer, (double) -1, answer);
      add(answer, layer, answer);
      scalMul(error, eta, error);
      wise(error, answer, answer);
}
