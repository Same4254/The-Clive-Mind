#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include "LayeredNetwork.hpp"

#endif

#ifndef MPI_H
#define MPI_H

#include <mpi.h>

#endif

class MPINetworkSendRecv {
    private:
        LayeredNetwork* network;

        int expectedLayers;
        int recievedLayers;

    public:
        MPINetworkSendRecv(LayeredNetwork* network) {
            this->network = network;

            expectedLayers = 0;
            recievedLayers = 0;
            for(int i = 0; i < network->getAmountOfLayers(); i++)
                if(network->getLayers()[i]->getParameters() != NULL) 
                    expectedLayers++;
        }

        void sendParameters() {
            for(int i = 0; i < network->getAmountOfLayers(); i++) {
                if(network->getLayers()[i]->getParameters() == NULL)
                    continue;

                MPI_Send(network->getLayers()[i]->getParameters(), network->getLayers()[i]->getParameterLength(), MPI_DOUBLE, 1, i, MPI_COMM_WORLD);
            }
        }

        void recvParameters() {
            for(int i = 0; i < network->getAmountOfLayers(); i++) {
                if(network->getLayers()[i]->getParameters() == NULL)
                    continue;

                MPI_Recv(network->getLayers()[i]->getParameters(), network->getLayers()[i]->getParameterLength(), MPI_DOUBLE, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        void sendGradientInfo() {

        }

        void recvGradientInfo() {
            
        }
};