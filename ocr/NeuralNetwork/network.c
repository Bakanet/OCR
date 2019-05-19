#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <err.h>
#include "matrix.h"
#include "../FilesTreatement/save.h"
#include "network.h"



//DEFINE CONSTANTS

size_t nbLayers = 3;
int myRandom = 5000;




/*****************/
/*Use the Network*/
/*****************/

//Init the Network with random values of weights and bias
Network *initNetwork(size_t *layers)
{
    srand(time(NULL));

    Network *network = malloc(sizeof(*network));
    if (network == NULL)
        errx(1, "malloc failed");
    if (layers == NULL)
    {
        network->weights1 = loadMatrix("./FilesTreatement/network", 0);
        network->weights2 = loadMatrix("./FilesTreatement/network", 1);
        network->bias1 = loadMatrix("./FilesTreatement/network", 2);
        network->bias2 = loadMatrix("./FilesTreatement/network", 3);
        network->layers = calloc(3, sizeof(size_t));
        network->layers[0] = network->weights1->width;
        network->layers[1] = network->bias1->height;
        network->layers[2] = network->bias2->height;
    }
    else
    {
        network->layers = layers;

        network->bias1 = initMatrix(layers[1], 1);
        randomizer(network->bias1->array, layers[1]);

        network->bias2 = initMatrix(layers[2], 1);
        randomizer(network->bias2->array, layers[2]);

        network->weights1 = initMatrix(layers[1], layers[0]);
        randomizer(network->weights1->array, layers[0] * layers[1]);

        network->weights2 = initMatrix(layers[2], layers[1]);
        randomizer(network->weights2->array, layers[1] * layers[2]);
    }

    return network;

}




//Create random values to put in the network
void randomizer(double *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        double value = rand() % myRandom;
        arr[i] = value / (double)(myRandom / 2) - 1;
    }
}




//Print all the elements of the network
void printNetwork(Network *network)
{
    printf("layers =\n%4ld,%4ld,%4ld\n\n",
            network->layers[0], network->layers[1], network->layers[2]);
    printf("bias1 =\n");
    printMatrix(network->bias1);
    printf("bias2 =\n");
    printMatrix(network->bias2);
    printf("weights1 =\n");
    printMatrix(network->weights1);
    printf("weights2 =\n");
    printMatrix(network->weights2);
    printf("input = \n");
    if (network->input != NULL)
        printMatrix(network->input);
    else
        printf("NULL");
    printf("output = \n");
    if (network->output != NULL)
        printMatrix(network->output);
    else
        printf("NULL");
}




//Free the Network and all of its components
void freeNetwork(Network *network)
{
    free(network->layers);
    freeMatrix(network->weights1);
    freeMatrix(network->weights2);
    freeMatrix(network->bias1);
    freeMatrix(network->bias2);
    free(network);
}




//set the inputs and the outputs to train the network
void setInput(Matrix **input, Matrix **output, char *inPath, char *outPath)
{
    size_t nbLines = countLines(inPath);
    for (size_t i = 0; i < nbLines; i++)
    {
        input[i] = loadMatrix(inPath, i);
        output[i] = loadMatrix(outPath, i);
    }
}




/*******************/
/*Train the Network*/
/*******************/

//Train the network during a given amount of epochs
void train(Network *network, size_t epochs, double eta, char *inPath, 
        char *outPath)
{
    size_t nbInputs = countLines(inPath);
    Matrix **input = calloc(nbInputs, sizeof(Matrix*));
    if (input == NULL)
        errx(1, "malloc fail");
    Matrix **output = calloc(nbInputs, sizeof(Matrix*));
    if (output == NULL)
        errx(1, "malloc fail");
    setInput(input, output, inPath, outPath);

    for (size_t i = 0; i < epochs; i++)
    {
        if (i % 5000 == 0)
            printf("%.2f\n", (double) i / (double) epochs * 100);
        size_t j = rand() % nbInputs;
        network->input = input[j];
        network->output = output[j];
        feedforward(network, eta);
    }

    int goodAnswers = 0;

    for (size_t i = 0; i < nbInputs; i++)
    {
        network->input = input[i];
        network->output = output[i];
        char result = detectCharacter(network);
        char expected = MatrixToChar(output[i], network->layers[2]);
        printf("expected : %c    result : %c\n", expected, result);
        if (expected == result)
            goodAnswers++;
    }
    printf("%i results are ok\n", goodAnswers);

    //disabled for presentation
    /*saveMatrix(network->weights1, "./FilesTreatement/network", "w");
    saveMatrix(network->weights2, "./FilesTreatement/network", "a");
    saveMatrix(network->bias1, "./FilesTreatement/network", "a");
    saveMatrix(network->bias2, "./FilesTreatement/network", "a");*/

    for (size_t i = 0; i < nbInputs; i++)
    {
        freeMatrix(input[i]);
        freeMatrix(output[i]);
    }
    free(input);
    free(output);
}




//Do one loop, adjusting the weights of the Network
void feedforward(Network *network, double eta)
{
    //FORWARD
    //Inputs to hidden layer

    Matrix *hiddenLayer = initMatrix(network->layers[1], 1);

    mul(network->weights1, network->input, hiddenLayer);
    add(network->bias1, hiddenLayer, hiddenLayer);
    sigmoid(hiddenLayer);

    //HiddenLayer to output

    Matrix *outputs = initMatrix(network->layers[2], 1);

    mul(network->weights2, hiddenLayer, outputs);
    add(network->bias2, outputs, outputs);
    sigmoid(outputs);

    //BACKWARD

    Matrix *errorOutput = initMatrix(network->layers[2], 1);
    Matrix *errorHL = initMatrix(network->layers[1], 1);
    Matrix *weights2transpose =
        initMatrix(network->weights2->width, network->weights2->height);

    scalMul(outputs, (double) -1, outputs);
    add(network->output, outputs, errorOutput);
    transpose(network->weights2, weights2transpose);
    mul(weights2transpose, errorOutput, errorHL);

    //CALCULATE GRADIENTS

    Matrix *gradientHL = initMatrix(network->layers[1], 1);
    Matrix *gradientOutput = initMatrix(network->layers[2], 1);

    gradient(eta, errorHL, hiddenLayer, gradientHL);
    gradient(eta, errorOutput, outputs, gradientOutput);

    //READJUST BIAS

    add(gradientHL, network->bias1, network->bias1);
    add(gradientOutput, network->bias2, network->bias2);

    //READJUST WEIGHTS

    Matrix *inputTranspose =
        initMatrix(network->input->width, network->input->height);
    Matrix *HLTranspose =
        initMatrix(hiddenLayer->width, hiddenLayer->height);
    Matrix *deltaHL =
        initMatrix(gradientHL->height, network->input->height);
    Matrix *deltaOutput =
        initMatrix(gradientOutput->height, hiddenLayer->height);

    transpose(network->input, inputTranspose);
    transpose(hiddenLayer, HLTranspose);
    mul(gradientHL, inputTranspose, deltaHL);
    add(deltaHL, network->weights1, network->weights1);
    mul(gradientOutput, HLTranspose, deltaOutput);
    add(deltaOutput, network->weights2, network->weights2);

    //FREE ALL

    freeMatrix(hiddenLayer);
    freeMatrix(outputs);
    freeMatrix(errorOutput);
    freeMatrix(errorHL);
    freeMatrix(weights2transpose);
    freeMatrix(gradientHL);
    freeMatrix(gradientOutput);
    freeMatrix(inputTranspose);
    freeMatrix(HLTranspose);
    freeMatrix(deltaHL);
    freeMatrix(deltaOutput);
}




//Once the network has trained, launch this function will return the character
//which represents the given matrix
char detectCharacter(Network *network)
{
    //Inputs to hidden Layer

    Matrix *hiddenLayer = initMatrix(network->layers[1], 1);

    mul(network->weights1, network->input, hiddenLayer);
    add(network->bias1, hiddenLayer, hiddenLayer);
    sigmoid(hiddenLayer);

    //HiddenLayer to output

    Matrix *outputs = initMatrix(network->layers[2], 1);

    mul(network->weights2, hiddenLayer, outputs);
    add(network->bias2, outputs, outputs);
    sigmoid(outputs);
    
    char result = MatrixToChar(outputs, network->layers[2]);

    freeMatrix(hiddenLayer);
    freeMatrix(outputs);

    return result;
}




//Convert an output matrix into a character
char MatrixToChar(Matrix *matrix, size_t len)
{
    int max = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (matrix->array[i] > matrix->array[max])
            max = i;
    }

    return (char) max + 33;
}
