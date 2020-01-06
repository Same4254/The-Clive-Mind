#include <stdio.h>
#include <iostream>
#include <time.h>

#include "mnistDB.hpp"

#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include "../LayeredNetwork/LayeredNetwork.hpp"

#endif

#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "../LayeredNetwork/SigmoidFunction.hpp"

#endif

Matrix* inputMatrix;
Matrix* answerMatrix;

int evaluate(LayeredNetwork* network) {
    int correct = 0;

    for(int i = 0; i < TEST_IMAGE_COUNT; i++) {
        inputMatrix->setData(testImages[i]);

        Matrix* output = network->feedForward(inputMatrix);
        if(output->greatestIndex() == testLabels[i]) {
            correct++;
        }
    }

    return correct;
}

int main() {
    loadDataset();
    srand(6854);

    inputMatrix = new Matrix(NULL, 28, 28);
    answerMatrix = new Matrix(10, 1, 0);

    // LayeredNetwork network(6, 1, 784, 1, 1, 10, 1);

    // network.getNetworkInformation()->setLearningRate(0.01);
    // network.getNetworkInformation()->setVelocityCoefficient(0.9);
    // network.getNetworkInformation()->setBatchSize(20);

    // FullyConnectedLayer* full1 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 0, 32);
    // ActivationLayer* act1 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 1, new SigmoidFunction());

    // FullyConnectedLayer* full2 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 2, 32);
    // ActivationLayer* act2 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 3, new SigmoidFunction());

    // FullyConnectedLayer* full3 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 4, 10);
    // ActivationLayer* act3 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 5, new SigmoidFunction());

    // network.getLayers()[0] = full1;
    // network.getLayers()[1] = act1;

    // network.getLayers()[2] = full2;
    // network.getLayers()[3] = act2;

    // network.getLayers()[4] = full3;
    // network.getLayers()[5] = act3;

    LayeredNetwork network(6, 1, 28, 28, 1, 10, 1);

    network.getNetworkInformation()->setLearningRate(0.01);
    network.getNetworkInformation()->setVelocityCoefficient(0.9);
    network.getNetworkInformation()->setBatchSize(20);

    ConvolutionalLayer* conv = new ConvolutionalLayer(network.getNetworkInformation(), network.getLayers(), 0, 1, 4, 1);
    ActivationLayer* act = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 1, new SigmoidFunction());

    // ConvolutionalLayer* conv2 = new ConvolutionalLayer(network.getNetworkInformation(), network.getLayers(), 2, 1, 4, 1);
    // ActivationLayer* act2 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 3, new SigmoidFunction());

    FullyConnectedLayer* full = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 2, 30);
    ActivationLayer* act3 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 3, new SigmoidFunction());

    FullyConnectedLayer* full2 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 4, 10);
    ActivationLayer* act4 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 5, new SigmoidFunction());

    network.getLayers()[0] = conv;
    network.getLayers()[1] = act;

    // network.getLayers()[2] = conv2;
    // network.getLayers()[3] = act2;

    network.getLayers()[2] = full;
    network.getLayers()[3] = act3;

    network.getLayers()[4] = full2;
    network.getLayers()[5] = act4;

    network.initialize();

    double percent = (evaluate(&network) / 100.0);

    printf("Initial Score: %f%%\n", percent);

    int lastAnswer = -1;

    clock_t start = clock();
    // for(int j = 0; j < 10; j++) {
        for(int i = 0; i < 60000; i++) {
            if(lastAnswer != -1)
                answerMatrix->set(lastAnswer, 0, 0);
            
            inputMatrix->setData(trainImages[i]);

            network.feedForward(inputMatrix);

            lastAnswer = trainLabels[i];
            answerMatrix->set(lastAnswer, 0, 1);

            network.calculateGradients(answerMatrix);

            if(network.getNetworkInformation()->incrementBatchIndex()) {
                network.update();
            }
        }

        clock_t end = clock();

        percent = (evaluate(&network) / 100.0);

        printf("Score: %f%%\n", percent);

        double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time: %f seconds\n", timeSpent);

        printf("------\n");
    // }
}