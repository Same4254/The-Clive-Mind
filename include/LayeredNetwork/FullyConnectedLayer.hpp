#ifndef FULLY_CONNECTED_LAYER_HPP
#define FULLY_CONNECTED_LAYER_HPP

#include "LayeredNetwork/Layer.hpp"
#include "LayeredNetwork/MomentumUpdater.hpp"
#include "LayeredNetwork/RMSUpdater.hpp"
#include "LayeredNetwork/AdamUpdater.hpp"

class FullyConnectedLayer : public Layer {
private:
    Matrix* weights;
    Matrix* biases;

    Matrix* weightGradient;
    Matrix* biasGradient;

    Updater* weightUpdater;
    Updater* biasUpdater;

public:
    FullyConnectedLayer(NetworkInformation* networkInformation, Layer** layers, int index, int numNodes);

    ~FullyConnectedLayer();

    void initialize();
    void postInitialize();

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update();
};
#endif