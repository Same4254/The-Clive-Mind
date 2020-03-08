#include <stdio.h>
#include <iostream>
#include <time.h>

#include "LayeredNetwork/NetworkBuilder.hpp"

#include "Databases/mnistDB.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"

// #include "LayeredNetwork/Layers/FullyConnectedLayer.hpp"
// #include "LayeredNetwork/Layers/ConvolutionalLayer.hpp"
// #include "LayeredNetwork/Layers/ActivationLayer.hpp"

// #include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"

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
    if(!loadDataset())
        std::cerr << "Failed to load dataset" << std::endl;

    srand(6854);

    NetworkBuilder builder;

    // builder.inputRows(28);
    // builder.inputCols(28);

    // builder.convolutionLayer(UpdaterID::Momentum, 1, 4, 1);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 30);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 10);
    // builder.activationLayer(ActivationID::Sigmoid);

    // builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 10);
    // builder.activationLayer(ActivationID::Sigmoid);

    // LayeredNetwork* network = builder.build();

    LayeredNetwork* network = builder.fromFile("state");

    network->getNetworkInformation().setLearningRate(0.01);
    network->getNetworkInformation().setVelocityCoefficient(0.9);
    network->getNetworkInformation().setBatchSize(20);

    inputMatrix = new Matrix(NULL, 28, 28);
    answerMatrix = new Matrix(10, 1, 0);

    double percent = evaluate(network) / 100.0;

    printf("Initial Score: %f%%\n", percent);

    int lastAnswer = -1;

    clock_t start = clock();
    // for(int j = 0; j < 2; j++) {
        for(int i = 0; i < 60000; i++) {
            if(lastAnswer != -1)
                answerMatrix->set(lastAnswer, 0, 0);
            
            inputMatrix->setData(trainImages[i]);

            network->feedForward(inputMatrix);

            lastAnswer = trainLabels[i];
            answerMatrix->set(lastAnswer, 0, 1);

            network->calculateGradients(answerMatrix);

            if(network->getNetworkInformation().incrementBatchIndex()) {
                network->update();
            }
        }

        clock_t end = clock();

        percent = (evaluate(network) / 100.0);

        printf("Score: %f%%\n", percent);

        double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time: %f seconds\n", timeSpent);

        printf("------\n");
    // }

    network->toFile("state");
}