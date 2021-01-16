#include <stdio.h>
#include <iostream>
#include <time.h>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"
#include "Databases/MnistDatabase.hpp"

#define MPI_H
#include <mpi.h>

double evaluateError(double* data, double* label, int length) {
    double error = 0;

    for(int i = 0; i < length; i++) 
        error += pow(data[i] - label[i], 2);

    return error;
}

int greatest(double* data, int length) {
    int index = 0;

    for(int k = 0; k < length; k++) 
        if(data[k] > data[index])
            index = k;

    return index;
}

int main() {
    MPI_Init(NULL, NULL);

    //Train worker networks
    Database* database = new MnistDatabase();

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int nodeCount;
    MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);

    srand(time(0) + id);

    if(id != 0) {
        //Build Thinkers
        NetworkBuilder builder;

        LayeredNetwork* network;
        if(id == 1) {
            network = builder.fromFile("state1");
            std::cout << "1" << std::endl;
        } else if(id == 2) {
            network = builder.fromFile("state2");
            std::cout << "2" << std::endl;
        } else if(id == 3) {
            network = builder.fromFile("state3");
            std::cout << "3" << std::endl;
        } else if(id == 4) {
            network = builder.fromFile("state4");
            std::cout << "4" << std::endl;
        } else if(id == 5) {
            network = builder.fromFile("state5");
            std::cout << "5" << std::endl;
        } else if(id == 6) {
            network = builder.fromFile("state6");
            std::cout << "6" << std::endl;
        } else if(id == 7) {
            network = builder.fromFile("state7");
            std::cout << "7" << std::endl;
        } else if(id == 8) {
            network = builder.fromFile("state8");
            std::cout << "8" << std::endl;
        }

        //Send data to blender
        for(int i = 0; i < database->getTestSize(); i++) {
            Matrix* output = network->feedForward(database->getTestData(i));

            MPI_Send(output->getData(), 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }

        for(int i = 0; i < database->getEpochSize(); i++) {
            Matrix* output = network->feedForward(database->getTrainingData(i));

            MPI_Send(output->getData(), 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }

        for(int i = 0; i < database->getEpochSize(); i++) {
            Matrix* output = network->feedForward(database->getTrainingData(i));

            MPI_Send(output->getData(), 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }

        for(int i = 0; i < database->getTestSize(); i++) {
            Matrix* output = network->feedForward(database->getTestData(i));

            MPI_Send(output->getData(), 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    } else {
        NetworkBuilder builder;
        builder.inputRows((nodeCount - 1) * 10);

        builder.fullyConnectedLayer(UpdaterID::RMS, 200);
        builder.activationLayer(ActivationID::Sigmoid);
        builder.fullyConnectedLayer(UpdaterID::RMS, 10);
        builder.activationLayer(ActivationID::Sigmoid);

        LayeredNetwork* network = builder.build();

        network->getNetworkInformation().setLearningRate(0.01);
        network->getNetworkInformation().setVelocityCoefficient(0.9);
        network->getNetworkInformation().setBatchSize(20);

        double* inputData = (double*) malloc(sizeof(double) * (nodeCount - 1) * 10);
        double error = 0;
        for(int i = 0; i < database->getTestSize(); i++) {
            for(int j = 1; j < nodeCount; j++)
                MPI_Recv(&inputData[10 * (j - 1)], 10, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            network->feedForward(inputData);

            error += evaluateError(network->getLayers()[network->getLayers().size() - 1]->getOutputInfo(), database->getTestLabel(i), 10);
        }

        error /= (2.0 * database->getTestSize());

        std::cout << "Initial Error: " << error << std::endl;

        for(int i = 0; i < database->getEpochSize(); i++) {
            for(int j = 1; j < nodeCount; j++)
                MPI_Recv(&inputData[10 * (j - 1)], 10, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            network->feedForward(inputData);
            network->calculateGradients(database->getTrainingLabel(i));

            if(network->getNetworkInformation().incrementBatchIndex())
                network->update();
        }

        for(int i = 0; i < database->getEpochSize(); i++) {
            for(int j = 1; j < nodeCount; j++)
                MPI_Recv(&inputData[10 * (j - 1)], 10, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            network->feedForward(inputData);
            network->calculateGradients(database->getTrainingLabel(i));

            if(network->getNetworkInformation().incrementBatchIndex())
                network->update();
        }

        error = 0;
        for(int i = 0; i < database->getTestSize(); i++) {
            for(int j = 1; j < nodeCount; j++)
                MPI_Recv(&inputData[10 * (j - 1)], 10, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            network->feedForward(inputData);

            error += evaluateError(network->getLayers()[network->getLayers().size() - 1]->getOutputInfo(), database->getTestLabel(i), 10);
        }

        error /= (2.0 * database->getTestSize());

        std::cout << "Final Error: " << error << std::endl;
    }

    MPI_Finalize();
    return 0;
}
