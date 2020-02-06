#ifndef ACTIVATION_LAYER_HPP
#define ACTIVATION_LAYER_HPP

#include "LayeredNetwork/Layer.hpp"
#include "LayeredNetwork/ActivationFunction.hpp"

class ActivationLayer: public Layer {
private: 
    ActivationFunction* function;

public:
    ActivationLayer(NetworkInformation* networkInformation, Layer** layers, int index, ActivationFunction* function);
    ~ActivationLayer();

    void initialize();
    void postInitialize();

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update() {}
};
#endif