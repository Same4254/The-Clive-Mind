#ifndef NETWORK_BUILDER
#define NETWORK_BUILDER

#include <vector>
#include <memory>

#include "LayeredNetwork/LayeredNetwork.hpp"
#include "LayeredNetwork/Layers/FullyConnectedLayer.hpp"
#include "LayeredNetwork/Layers/ActivationLayer.hpp"
#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"

class NetworkBuilder {
private:
    std::unique_ptr<LayeredNetwork> network;

    int inputMatrixCount, inputNRows, inputNCols;

    void createNetwork();

public:
    NetworkBuilder();
    ~NetworkBuilder();

    LayeredNetwork* build();

    NetworkBuilder& fullyConnectedLayer(UpdaterID id, int numNodes);
    // NetworkBuilder& convLayer(UpdaterID id, int kernalCount, int kernalSize, int stride);
    NetworkBuilder& activationLayer(ActivationID id);

    void matrixCount(int inputMatrixCount) { this->inputMatrixCount = inputMatrixCount; }
    void inputRows(int inputNRows) { this->inputNRows = inputNRows; }
    void inputCols(int inputNCols) { this->inputNCols = inputNCols; }
};
#endif