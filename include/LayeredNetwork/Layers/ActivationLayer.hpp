#ifndef ACTIVATION_LAYER_HPP
#define ACTIVATION_LAYER_HPP

#include "LayeredNetwork/Layers/Layer.hpp"
#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"
#include "LayeredNetwork/ActivationFunctions/ReluFunction.hpp"

class ActivationLayer: public Layer {
private: 
    ActivationFunction* function;

public:
    ActivationLayer(NetworkInformation& networkInformation, int index, ActivationID activationID);
    ~ActivationLayer();

    void initialize();
    void postInitialize();

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update() {}
};
#endif