#ifndef ACTIVATION_LAYER_HPP
#define ACTIVATION_LAYER_HPP

#include "LayeredNetwork/Layers/Layer.hpp"
#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/ReluFunction.hpp"

class ActivationLayer: public Layer {
private: 
    ActivationFunction* function;
    ActivationID activationID;

public:
    ActivationLayer(NetworkInformation& networkInformation, int index, ActivationID activationID);
    ~ActivationLayer();

    void initialize();
    void postInitialize();

    void writeConstructInfo(FILE* file);
    void writeState(FILE* file);
    void loadState(FILE* file);

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update() {}
};
#endif