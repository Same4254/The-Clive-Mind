#ifndef ACTIVATION_LAYER_HPP
#define ACTIVATION_LAYER_HPP

#include <rapidjson/document.h>

#include "LayeredNetwork/Layers/Layer.hpp"
#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/ReluFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/SoftplusFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/LeakyReluFunction.hpp"

class ActivationLayer: public Layer {
private: 
    ActivationFunction* function;
    ActivationID activationID;

public:
    ActivationLayer(NetworkInformation& networkInformation, int index, ActivationID activationID);
    ~ActivationLayer();

    void initialize();
    void postInitialize();

    void writeStructureToFile(rapidjson::Value& layerJSONObject, rapidjson::Document::AllocatorType& allocator);
    void writeStateToFile(FILE* file);
    bool loadStateFromFile(FILE* file);

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update() {}
};
#endif