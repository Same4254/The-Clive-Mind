#include <stdio.h>
#include <iostream>
#include <time.h>

#include "mnistDB.hpp"
#include "../LayeredNetwork/LayeredNetwork.hpp"

using namespace std;

Matrix* inputMatrix;
Matrix* answerMatrix;

int greatest(int numElements, double* data) {
    int index = 0; 

    for(int i = 1; i < numElements; i++)
        if(data[index] < data[i])
            index = i;

    return index;
}

int evaluate(LayeredNetwork* network) {
    int correct = 0;

    for(int i = 0; i < TEST_IMAGE_COUNT; i++) {
        inputMatrix->setDataUNSAFE(testImages[i]);

        Matrix* output = network->feedForward(inputMatrix);
        if(greatest(10, output->getData()) == testLabels[i]) {
            correct++;
        }
    }

    return correct;
}

int main() {
    loadDataset();
    srand(764);

    inputMatrix = new Matrix(28, 28);
    answerMatrix = new Matrix(10, 1);

    ActivationFunction* reluFunction = new ReluFunction();

    LayeredNetwork network(4, 1, 28, 28, 1, 10, 1);

    ConvolutionalLayer conv(network.layers, 0, 1, 3, 1);
    FlatteningLayer flatten(network.layers, 1);
    FullyConnectedLayer full1(network.layers, 2, 40);
    FullyConnectedLayer full2(network.layers, 3, 10);

    network.layers[0] = &conv;
    network.layers[1] = &flatten;
    network.layers[2] = &full1;
    network.layers[3] = &full2;

    network.initialize();

    double percent = (evaluate(&network) / 100.0);

    printf("Initial Score: %f%%\n", percent);

    int lastAnswer = -1;
    double answers[10];

    for(int i = 0; i < 10; i++)
        answers[i] = 0;

    clock_t start = clock();
    for(int j = 0; j < 10; j++) {
        for(int i = 0; i < 60000; i++) {
            if(lastAnswer != -1)
                answers[lastAnswer] = 0;
            
            inputMatrix->setDataUNSAFE(trainImages[i]);

            network.feedForward(inputMatrix);

            lastAnswer = trainLabels[i];

            answers[lastAnswer] = 1;

            answerMatrix->setDataUNSAFE(answers);
            network.backpropogate(answerMatrix);

            if(j == 3 && i == 25000) {
                // printf("here");
            }
        }

        clock_t end = clock();

        percent = (evaluate(&network) / 100.0);

        printf("Score: %f%%\n", percent);

        double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time: %f seconds\n", timeSpent);

        printf("------\n");
    }



    // loadDataset();

    // srand(2348);

    // int layers[] = {784, 32, 32, 10};
    // FullyConnectedNeuralNetwork connected(4, layers);

    // double percent = (evaluate(&connected) / 100.0);

    // printf("Initial Score: %f%%\n", percent);

    // int lastAnswer = -1;
    // double answers[10];

    // for(int i = 0; i < 10; i++)
    //     answers[i] = 0;

    // clock_t start = clock();
    // // for(int j = 0; j < 4; j++) {
    //     for(int i = 0; i < 60000; i++) {
    //         if(lastAnswer != -1)
    //             answers[lastAnswer] = 0;
            
    //         connected.feedForward(trainImages[i]);

    //         lastAnswer = trainLabels[i];

    //         answers[lastAnswer] = 1;

    //         connected.backpropogate(answers);
    //     }

    //     clock_t end = clock();

    //     percent = (evaluate(&connected) / 100.0);

    //     printf("Score: %f%%\n", percent);

    //     double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

    //     printf("Time: %f seconds\n", timeSpent);

    //     printf("------\n");
    // // }
}