#ifndef FULLY_CONNECTED_LAYER_HPP
#define FULLY_CONNECTED_LAYER_HPP

#include "LayeredNetwork/Layers/Layer.hpp"

class FullyConnectedLayer : public Layer {
private:
    Matrix* weights;
    Matrix* biases;

    Matrix* weightGradient;
    Matrix* biasGradient;

    UpdaterID updaterID;
    std::unique_ptr<Updater> weightUpdater, biasUpdater;

public:
    FullyConnectedLayer(NetworkInformation& networkInformation, UpdaterID updaterID, int index, int numNodes);
    ~FullyConnectedLayer();

    void initialize();
    void postInitialize();
    
    void writeConstructInfo(FILE* file);
    void writeState(FILE* file);
    void loadState(FILE* file);

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update();
};
#endif