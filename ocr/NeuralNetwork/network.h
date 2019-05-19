#ifndef WORKINGNETWORK_H
#define WORKINGNETWORK_H

typedef struct Network Network;
struct Network
{
   size_t *layers;
   Matrix *weights1;
   Matrix *weights2;
   Matrix *bias1;
   Matrix *bias2;
   Matrix *input;
   Matrix *output;
};

//Using the network

Network *initNetwork(size_t *layers);
void randomizer(double *arr, size_t len);
void freeNetwork(Network *network);
void printNetwork(Network *network);
void setInput(Matrix **input, Matrix **output, char *inPath, char *outPath);

//Train the network

void train(Network *network, size_t epochs, double eta, char *inPath, 
        char *outPath);
void feedforward(Network *network, double eta);
char detectCharacter(Network *network);
char MatrixToChar(Matrix *matrix, size_t len);

#endif
