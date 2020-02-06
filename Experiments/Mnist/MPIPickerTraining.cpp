// #include <stdio.h>
// #include <iostream>
// #include <time.h>

// #include "mnistDB.hpp"

// #ifndef LAYERED_NETWORK_HPP
// #define LAYERED_NETWORK_HPP

// #include "../LayeredNetwork/LayeredNetwork.hpp"

// #endif

// #ifndef SIGMOID_FUNCTION_HPP
// #define SIGMOID_FUNCTION_HPP

// #include "../LayeredNetwork/SigmoidFunction.hpp"

// #endif

// #define MPI_H
// #include <mpi.h>

// int evaluate(LayeredNetwork* network, Matrix* inputMatrix) {
//     int correct = 0;

//     for(int i = 0; i < TEST_IMAGE_COUNT; i++) {
//         inputMatrix->setData(testImages[i]);

//         Matrix* output = network->feedForward(inputMatrix);
//         if(output->greatestIndex() == testLabels[i])
//             correct++;
//     }

//     return correct;
// }

// int main() {
//     MPI_Init(NULL, NULL);

//     if(!loadDataset()) {
//         printf("Could not load dataset!");
//         return -1;
//     }

//     // srand(6854);
//     int id;
//     MPI_Comm_rank(MPI_COMM_WORLD, &id);

//     int nodeCount;
//     MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);

//     srand(time(0) + id);

//     Matrix* inputMatrix = new Matrix(NULL, 784, 1);
//     Matrix* answerMatrix = new Matrix(10, 1, 0);

//     LayeredNetwork network(6, 1, 784, 1, 1, 10, 1);

//     network.getNetworkInformation()->setLearningRate(0.01);
//     network.getNetworkInformation()->setVelocityCoefficient(0.9);
//     network.getNetworkInformation()->setBatchSize(20);

//     FullyConnectedLayer* full1 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 0, 32);
//     ActivationLayer* act1 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 1, new SigmoidFunction());

//     FullyConnectedLayer* full2 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 2, 32);
//     ActivationLayer* act2 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 3, new SigmoidFunction());

//     FullyConnectedLayer* full3 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 4, 10);
//     ActivationLayer* act3 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 5, new SigmoidFunction());

//     network.getLayers()[0] = full1;
//     network.getLayers()[1] = act1;

//     network.getLayers()[2] = full2;
//     network.getLayers()[3] = act2;

//     network.getLayers()[4] = full3;
//     network.getLayers()[5] = act3;

//     network.initialize();

//     double percent = evaluate(&network, inputMatrix) / 100.0;
//     int lastAnswer = -1;

//     printf("Node #%d, Initial Score: %f%%\n", id, percent);

//     for(int i = 0; i < 60000; i++) {
//         if(lastAnswer != -1)
//             answerMatrix->set(lastAnswer, 0, 0);
        
//         inputMatrix->setData(trainImages[i]);

//         network.feedForward(inputMatrix);

//         lastAnswer = trainLabels[i];
//         answerMatrix->set(lastAnswer, 0, 1);

//         network.calculateGradients(answerMatrix);

//         if(network.getNetworkInformation()->incrementBatchIndex()) {
//             network.update();
//         }
//     }

//     percent = (evaluate(&network, inputMatrix) / 100.0);
//     printf("Node #%d, Final Score: %f%%\n", id, percent);

    

//     MPI_Finalize();
//     return 0;
// }
