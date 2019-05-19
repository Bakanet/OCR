#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include "../NeuralNetwork/matrix.h"
#include "save.h"


//Save a matrix into the file given in the path
int saveMatrix(Matrix *mat, char *path, char *mode)
{
    FILE *f;

    f = fopen(path, mode);
    if (f == NULL)
        return -1;

    fprintf(f, "%lu, %lu, [", mat->height, mat->width);
    size_t i;

    for (i = 0; i < mat->height * mat->width - 1; i++)
    {
        if (mat->array[i] >= 0)
            fprintf(f, "+");
        fprintf(f, "%.5f, ", mat->array[i]);
    }
    if (mat->array[i] >= 0)
        fprintf(f, "+");
    fprintf(f, "%.5f]\n", mat->array[i]);
    fclose(f);

    return 0;
}




//load the Matrix that has been saved in the code
Matrix *loadMatrix(char *path, size_t line)
{
    FILE *f;
    size_t height = 0;
    size_t width = 0;
    Matrix *result = NULL;

    f = fopen(path, "r");
    if (f == NULL)
        errx(-1, "loadMatrix: pointer is NULL");

    char c = fgetc(f);
    size_t l = 0;
    double value, increment;
    int sign;

    while (c != EOF)
    {
        if (l == line)
        {
            while (c >= '0' && c <= '9')
            {
                height *= 10;
                height += c - '0';
                c = fgetc(f);
            }
            while (c < '0' || c > '9')
                c = fgetc(f);
            while (c >= '0' && c <= '9')
            {
                width *= 10;
                width += c - '0';
                c = fgetc(f);
            }
            result = initMatrix(height, width);
            
            size_t i = 0;

            while (c != '[')
                c = fgetc(f);
            
            c = fgetc(f);

            while (c != ']' && c != EOF)
            {
                value = 0;
                sign = 0;
                
                while (c != '+' && c != '-')
                {
                    c = fgetc(f);
                }
                
                sign = (c == '+');

                c = fgetc(f);

                while (c != '.')
                {
                    value *= 10;
                    value += (c - '0');
                    c = fgetc(f);
                }

                c = fgetc(f);

                increment = 0.1;
                while (c != ',' && c != ']')
                {
                    value += (c - '0') * increment;
                    increment /= 10;
                    c = fgetc(f);
                }
                
                if (!sign)
                    value *= -1;

                result->array[i] = value;
                i++;
            }

            while (c != '\n' && c != EOF)
                c = fgetc(f);
        }

        if (c == '\n')
            l++;

        c = fgetc(f);
    }
    fclose(f);

    return result;
}





//Count the number of lines of the file
size_t countLines(char *path)
{
    FILE *f;
    size_t l = 0;

    f = fopen(path, "r");
    if (f == NULL)
        errx(-1, "countlines: pointer is NULL");
    char c = fgetc(f);

    while (c != EOF)
    {
        if (c == '\n')
            l++;
        c = fgetc(f);
    }
    fclose(f);

    return l;
}
