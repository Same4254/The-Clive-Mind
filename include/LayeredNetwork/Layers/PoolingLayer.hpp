#ifndef POOLING_LAYER_HPP
#define POOLING_LAYER_HPP

#include <vector>
#include "LayeredNetwork/Layers/Layer.hpp"

class PoolingLayer: public Layer {
private:
    Matrix* poolIndecies;

    int size;
    int numPoolIndecies;

public:
    PoolingLayer(NetworkInformation& networkInformation, int index, int size);

    void initialize();

    Matrix* feedForward(Matrix* input);
    Matrix* backpropogate(Matrix* error);
};
#endif