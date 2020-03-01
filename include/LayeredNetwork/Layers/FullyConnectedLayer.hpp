#ifndef FULLY_CONNECTED_LAYER_HPP
#define FULLY_CONNECTED_LAYER_HPP

#include "LayeredNetwork/Layers/Layer.hpp"

#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"
#include "LayeredNetwork/Updaters/AdamUpdater.hpp"
#include "LayeredNetwork/Updaters/RMSUpdater.hpp"

class FullyConnectedLayer : public Layer {
private:
    Matrix* weights;
    Matrix* biases;

    Matrix* weightGradient;
    Matrix* biasGradient;

    UpdaterID updaterID;
    Updater* weightUpdater;
    Updater* biasUpdater;

public:
    FullyConnectedLayer(NetworkInformation& networkInformation, UpdaterID updaterID, int index, int numNodes);
    ~FullyConnectedLayer();

    void initialize();
    void postInitialize();

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update();
};
#endif