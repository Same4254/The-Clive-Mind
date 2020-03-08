#include <stdio.h>
#include <iostream>
#include <time.h>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"
#include "Databases/MnistDatabase.hpp"

int main() {
    srand(6854);

    NetworkBuilder builder;

    builder.inputRows(784);

    builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    builder.activationLayer(ActivationID::Sigmoid);
    builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    builder.activationLayer(ActivationID::Sigmoid);
    builder.fullyConnectedLayer(UpdaterID::Momentum, 10);
    builder.activationLayer(ActivationID::Sigmoid);

    LayeredNetwork* network = builder.build();

    network->getNetworkInformation().setLearningRate(0.01);
    network->getNetworkInformation().setVelocityCoefficient(0.9);
    network->getNetworkInformation().setBatchSize(20);

    Database* database = new MnistDatabase();

    double percent = network->evaluate(database) * 100.0;

    printf("Initial Score: %f%%\n", percent);

    clock_t start = clock();
    network->trainEpoch(database);
    clock_t end = clock();

    percent = network->evaluate(database) * 100.0;

    printf("Final Score: %f%%\n", percent);

    double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", timeSpent);
}