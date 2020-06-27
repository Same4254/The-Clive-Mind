#ifndef POOLING_LAYER_HPP
#define POOLING_LAYER_HPP

#include <rapidjson/document.h>

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

    void toFile(FILE* file);
    void fromFile(FILE* file);

    void writeStructureToFile(rapidjson::Value& layerJSONObject, rapidjson::Document::AllocatorType& allocator);

    Matrix* feedForward(Matrix* input);
    Matrix* backpropogate(Matrix* error);
};
#endif