#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include "LayeredNetwork/ConvolutionalLayer.hpp"
#include "LayeredNetwork/ActivationLayer.hpp"
#include "LayeredNetwork/FullyConnectedLayer.hpp"
#include "LayeredNetwork/NetworkInformation.hpp"

#include <stdio.h>

class LayeredNetwork {
private:
    NetworkInformation* networkInformation;
    Layer** layers;
    int amountOfLayers;

    int inputMatrixCount;
    int inputNRows;
    int inputNCols;

    int outputMatrixCount;
    int outputNRows;
    int outputNCols;

    Matrix* error;

public:
    LayeredNetwork(int amountOfLayers, int inputMatrixCount, int inputNRows, int inputNCols, int outputMatrixCount, int outputNRows, int outputNCols);

    ~LayeredNetwork();

    void initialize();

    Matrix* feedForward(Matrix* input);
    void calculateGradients(Matrix* labels);
    void update();

    void writeState(FILE* fd);

    NetworkInformation* getNetworkInformation();
    Matrix* getOutput();

    int getAmountOfLayers();
    Layer** getLayers();
};
#endif