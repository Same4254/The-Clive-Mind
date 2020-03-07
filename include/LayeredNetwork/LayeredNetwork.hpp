#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include <stdio.h>
#include <memory>
#include <vector>

#include "LayeredNetwork/NetworkInformation.hpp"
#include "LayeredNetwork/Layers/Layer.hpp"

class LayeredNetwork {
private:
    NetworkInformation networkInformation;
    std::vector<std::unique_ptr<Layer>> layers;

    int inputMatrixCount;
    int inputNRows;
    int inputNCols;

    int outputMatrixCount;
    int outputNRows;
    int outputNCols;

public:
    LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols);
    ~LayeredNetwork();

    void initialize();
    void toFile(char* filename);

    Matrix* feedForward(Matrix* input);
    void calculateGradients(Matrix* labels);
    void update();

    NetworkInformation& getNetworkInformation();
    Matrix* getOutput();

    std::vector<std::unique_ptr<Layer>>& getLayers();
};
#endif