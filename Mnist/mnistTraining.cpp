#include <stdio.h>
#include <iostream>
#include <time.h>

#include "mnistDB.hpp"

#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include "../LayeredNetwork/LayeredNetwork.hpp"

#endif

// #include "../LayeredNetwork/MPINetworkSendRecv.cpp"

// #ifndef MPI_H
// #define MPI_H

// #include <mpi.h>

// #endif

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
    // MPI_Init(NULL, NULL);

    // int id;
    // MPI_Comm_rank(MPI_COMM_WORLD, &id);

    loadDataset();
    srand(6854);

    inputMatrix = new Matrix(NULL, 784, 1);
    answerMatrix = new Matrix(10, 1, 0);

    LayeredNetwork network(3, 1, 784, 1, 1, 10, 1);

    network.getNetworkInformation()->setLearningRate(0.01);
    network.getNetworkInformation()->setVelocityCoefficient(0.9);
    network.getNetworkInformation()->setBatchSize(20);

    FullyConnectedLayer full1(network.getNetworkInformation(), network.getLayers(), 0, 32);
    FullyConnectedLayer full2(network.getNetworkInformation(), network.getLayers(), 1, 32);
    FullyConnectedLayer full3(network.getNetworkInformation(), network.getLayers(), 2, 10);

    network.getLayers()[0] = &full1;
    network.getLayers()[1] = &full2;
    network.getLayers()[2] = &full3;

    network.initialize();
    
    double percent = (evaluate(&network) / 100.0);

    printf("Master Initial Score: %f%%\n", percent);

    int lastAnswer = -1;

    clock_t start = clock();
    // for(int j = 0; j < 10; j++) {
        for(int i = 0; i < 60000; i++) {
            if(lastAnswer != -1)
                answerMatrix->set(lastAnswer, 0, 0);
                // answers[lastAnswer] = 0;
            
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

        printf("Master Score: %f%%\n", percent);

        double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Master Time: %f seconds\n", timeSpent);

        printf("Master ------\n");

        // sendRecv.sendParameters();
    // }


    // MPINetworkSendRecv sendRecv(&network);

    // if(id == 0) {
    //     double percent = (evaluate(&network) / 100.0);

    //     printf("Master Initial Score: %f%%\n", percent);

    //     int lastAnswer = -1;

    //     clock_t start = clock();
    //     for(int j = 0; j < 10; j++) {
    //         for(int i = 0; i < 20000; i++) {
    //             if(lastAnswer != -1)
    //                 answerMatrix->set(lastAnswer, 0, 0);
    //                 // answers[lastAnswer] = 0;
                
    //             inputMatrix->setDataUNSAFE(trainImages[i]);

    //             network.feedForward(inputMatrix);

    //             lastAnswer = trainLabels[i];

    //             answerMatrix->set(lastAnswer, 0, 1);

    //             // answerMatrix->setDataUNSAFE(answers);
    //             // network.backpropogate(answerMatrix);
    //             network.calculateGradients(answerMatrix);

    //             if(network.getNetworkInformation()->incrementBatchIndex())
    //                 network.update();
    //         }

    //         clock_t end = clock();

    //         percent = (evaluate(&network) / 100.0);

    //         printf("Master Score: %f%%\n", percent);

    //         double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

    //         printf("Master Time: %f seconds\n", timeSpent);

    //         printf("Master ------\n");

    //         sendRecv.sendParameters();
    //     }
    // } else {
    //     sendRecv.recvParameters();

    //     double percent = (evaluate(&network) / 100.0);

    //     printf("Slave Initial Score: %f%%\n", percent);

    //     int lastAnswer = -1;
    //     double answers[10];

    //     for(int i = 0; i < 10; i++)
    //         answers[i] = 0;

    //     clock_t start = clock();
    //     for(int j = 0; j < 10; j++) {
    //         for(int i = 0; i < 20000; i++) {
    //             if(lastAnswer != -1)
    //                 answers[lastAnswer] = 0;
                
    //             inputMatrix->setDataUNSAFE(trainImages[i]);

    //             network.feedForward(inputMatrix);

    //             lastAnswer = trainLabels[i];

    //             answers[lastAnswer] = 1;

    //             answerMatrix->setDataUNSAFE(answers);
    //             // network.backpropogate(answerMatrix);
    //             network.calculateGradients(answerMatrix);

    //             if(network.getNetworkInformation()->incrementBatchIndex())
    //                 network.update();
    //         }

    //         clock_t end = clock();

    //         percent = (evaluate(&network) / 100.0);

    //         printf("Slave Score: %f%%\n", percent);

    //         double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

    //         printf("Slave Time: %f seconds\n", timeSpent);

    //         printf("Slave ------\n");

    //         sendRecv.sendParameters();
    //     }
    // }

    // MPI_Finalize();
}