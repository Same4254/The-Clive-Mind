#include <stdio.h>
#include <iostream>
#include <time.h>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"
#include "Databases/MnistDatabase.hpp"

int main() {
    srand(6854);
    // srand(time(0));

    NetworkBuilder builder;

    // builder.inputRows(784);

    // builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 32);
    // builder.activationLayer(ActivationID::Sigmoid);
    // builder.fullyConnectedLayer(UpdaterID::Momentum, 10);
    // builder.activationLayer(ActivationID::Sigmoid);

    LayeredNetwork* network = builder.loadStructurefromFile("Models/BasicMNIST.json");
    LayeredNetwork* network2 = builder.loadStructurefromFile("Models/BasicMNIST.json");
    // network->loadStateFromFile("Models/BasicMNIST.state");

    network->getNetworkInformation().setLearningRate(0.01);
    network->getNetworkInformation().setVelocityCoefficient(0.9);
    network->getNetworkInformation().setBatchSize(20);

    Database* database = new MnistDatabase();

    double error = network->evaluate(database);

    printf("Initial Error: %f\n", error);

    clock_t start = clock();
    network->trainEpoch(database);
    clock_t end = clock();

    error = network->evaluate(database);

    printf("Final Error: %f\n", error);

    double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", timeSpent);

    network2->copyState(network);

    error = network2->evaluate(database);

    printf("Final Error 2: %f\n", error);

    // network->writeStateToFile("Models/BasicMNIST.state");
}