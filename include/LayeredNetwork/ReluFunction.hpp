#ifndef RELU_FUNCTION_HPP
#define RELU_FUNCTION_HPP

#include "LayeredNetwork/ActivationFunction.hpp"

class ReluFunction: public ActivationFunction {
public:
    ReluFunction();
    ~ReluFunction();

    double applyFunction(double x);
    double applyDerivativeFunction(double x);
};
#endif