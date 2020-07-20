#ifndef ACTIVATION_LAYER_HPP
#define ACTIVATION_LAYER_HPP

#include <rapidjson/document.h>

#include "LayeredNetwork/Layers/Layer.hpp"
#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/ReluFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/SoftplusFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/LeakyReluFunction.hpp"

//TODO Needs updating
class ActivationLayer: public Layer {
private: 
    ActivationFunction* function;
    ActivationID activationID;

public:
    ActivationLayer(NetworkInformation& networkInformation, ActivationID activationID, int index);
    ~ActivationLayer();

    void initialize();
    void postInitialize();

    void appendCopy(NetworkInformation& networkInformation);

    void writeStructureToFile(rapidjson::Value& layerJSONObject, rapidjson::Document::AllocatorType& allocator);
    void writeStateToFile(FILE* file);
    bool loadStateFromFile(FILE* file);

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update();
};
#endif