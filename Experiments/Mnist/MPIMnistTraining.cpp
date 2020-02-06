// #include <stdio.h>
// #include <iostream>
// #include <time.h>

// #include "mnistDB.hpp"

// #ifndef LAYERED_NETWORK_HPP
// #define LAYERED_NETWORK_HPP

// #include "../LayeredNetwork/LayeredNetwork.hpp"

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

//     srand(6854);

//     int id;
//     MPI_Comm_rank(MPI_COMM_WORLD, &id);

//     int nodeCount;
//     MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);

//     int workerCount = nodeCount - 1;

//     int localBatchSize = 10;
//     int globalBatchsize = localBatchSize * workerCount;

//     // LayeredNetwork network(3, 1, 784, 1, 1, 10, 1);

//     // network.getNetworkInformation()->setLearningRate(0.01);
//     // network.getNetworkInformation()->setVelocityCoefficient(0.9);
//     // network.getNetworkInformation()->setBatchSize(localBatchSize);

//     // FullyConnectedLayer* full1 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 0, 32);
//     // FullyConnectedLayer* full2 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 1, 32);
//     // FullyConnectedLayer* full3 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 2, 10);

//     // network.getLayers()[0] = full1;
//     // network.getLayers()[1] = full2;
//     // network.getLayers()[2] = full3;

//     LayeredNetwork network(7, 1, 28, 28, 1, 10, 1);

//     ConvolutionalLayer* conv = new ConvolutionalLayer(network.getNetworkInformation(), network.getLayers(), 0, 1, 4, 1);
//     ActivationLayer* act = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 1, new SigmoidFunction());
//     FullyConnectedLayer* full = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 3, 30);
//     ActivationLayer* act2 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 4, new SigmoidFunction());
//     FullyConnectedLayer* full2 = new FullyConnectedLayer(network.getNetworkInformation(), network.getLayers(), 5, 10);
//     ActivationLayer* act3 = new ActivationLayer(network.getNetworkInformation(), network.getLayers(), 6, new SigmoidFunction());

//     network.getLayers()[0] = conv;
//     network.getLayers()[1] = act;
//     network.getLayers()[2] = flat;
//     network.getLayers()[3] = full;
//     network.getLayers()[4] = act2;
//     network.getLayers()[5] = full2;
//     network.getLayers()[6] = act3;

//     network.initialize();

//     Matrix* inputMatrix = new Matrix(NULL, 28, 28);

//     for(int i = 0; i < network.getAmountOfLayers(); i++) {
//         MPI_Bcast(network.getLayers()[i]->getParameters(), network.getLayers()[i]->getParameterLength(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
//     }

//     // double percent = (evaluate(&network, inputMatrix) / 100.0);
//     // printf("Initial Score: %f%%\n", percent);

//     int lastAnswer = -1;

//     int batches = 3000;

//     if(id == 0) {//master
//         for(int batch = 0; batch < batches; batch++) {
//             for(int i = 0; i < network.getAmountOfLayers(); i++)
//                 for(int j = 0; j < network.getLayers()[i]->getGradientLength(); j++)
//                     network.getLayers()[i]->getGradientInfo()[j] = 0;


//             for(int i = 0; i < network.getAmountOfLayers(); i++)
//                 MPI_Reduce(MPI_IN_PLACE, network.getLayers()[i]->getGradientInfo(), network.getLayers()[i]->getGradientLength(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

//             network.update();

//             for(int i = 0; i < network.getAmountOfLayers(); i++) 
//                 MPI_Bcast(network.getLayers()[i]->getParameters(), network.getLayers()[i]->getParameterLength(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
//         }
//     } else {//worker
//         Matrix* answerMatrix = new Matrix(10, 1, 0);

//         for(int batch = 0; batch < batches; batch++) {
//             int startIndex = (batch * globalBatchsize) + ((id - 1) * localBatchSize);
//             for(int i = startIndex; i < startIndex + localBatchSize; i++) {
//                 if(lastAnswer != -1)
//                     answerMatrix->set(lastAnswer, 0, 0);
                
//                 inputMatrix->setData(trainImages[i]);

//                 network.feedForward(inputMatrix);

//                 lastAnswer = trainLabels[i];
//                 answerMatrix->set(lastAnswer, 0, 1);

//                 network.calculateGradients(answerMatrix);

//                 if(network.getNetworkInformation()->incrementBatchIndex()) {
//                     for(int i = 0; i < network.getAmountOfLayers(); i++)
//                         MPI_Reduce(network.getLayers()[i]->getGradientInfo(), NULL, network.getLayers()[i]->getGradientLength(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

//                     for(int i = 0; i < network.getAmountOfLayers(); i++)
//                         MPI_Bcast(network.getLayers()[i]->getParameters(), network.getLayers()[i]->getParameterLength(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
//                 }
//             }
//         }
//     }

//     // if(id == 0) {
//     //     double percent = (evaluate(&network, inputMatrix) / 100.0);
//     //     printf("Master Final Score: %f%%\n", percent);
//     // }

//     // if(id == 1) {
//     //     double percent = (evaluate(&network, inputMatrix) / 100.0);
//     //     printf("Worker Final Score: %f%%\n", percent);
//     // }

//     MPI_Finalize();

//     return 0;
// }
