#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "LayeredNetwork/ActivationFunction.hpp"

class SigmoidFunction: public ActivationFunction {
public:
    SigmoidFunction();

    double applyFunction(double x);
    double applyDerivativeFunction(double x);
};
#endif