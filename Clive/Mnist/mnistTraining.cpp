#include <stdio.h>
#include <iostream>
#include <time.h>

#include "mnistDB.hpp"
#include "../FullyConnectedNeuralNetwork.cpp"

using namespace std;

int greatest(int numElements, double* data) {
    int index = 0; 

    for(int i = 1; i < numElements; i++)
        if(data[index] < data[i])
            index = i;

    return index;
}

int evaluate(FullyConnectedNeuralNetwork* network) {
    int correct = 0;

    for(int i = 0; i < TEST_IMAGE_COUNT; i++) {
        Matrix* output = network->feedForward(testImages[i]);
        if(greatest(10, output->getData()) == testLabels[i]) {
            correct++;
        }
    }

    return correct;
}

int main() {
    loadDataset();

    srand(2348);

    int layers[] = {784, 32, 32, 10};
    FullyConnectedNeuralNetwork connected(4, layers);

    double percent = (evaluate(&connected) / 100.0);

    printf("Initial Score: %f%%\n", percent);

    int lastAnswer = -1;
    double answers[10];

    for(int i = 0; i < 10; i++)
        answers[i] = 0;

    clock_t start = clock();
    // for(int j = 0; j < 4; j++) {
        for(int i = 0; i < 60000; i++) {
            if(lastAnswer != -1)
                answers[lastAnswer] = 0;
            
            connected.feedForward(trainImages[i]);

            lastAnswer = trainLabels[i];

            answers[lastAnswer] = 1;

            connected.backpropogate(answers);
        }

        clock_t end = clock();

        percent = (evaluate(&connected) / 100.0);

        printf("Score: %f%%\n", percent);

        double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time: %f seconds\n", timeSpent);

        printf("------\n");
    // }
}