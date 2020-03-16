#ifndef LEAKY_RELU_FUNCTION_HPP
#define LEAKY_RELU_FUNCTION_HPP

#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

class LeakyReluFunction: public ActivationFunction {
public:
    LeakyReluFunction();
    ~LeakyReluFunction();

    double applyFunction(double x);
    double applyDerivativeFunction(double x);
};
#endif