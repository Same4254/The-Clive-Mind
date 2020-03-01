#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

class SigmoidFunction: public ActivationFunction {
public:
    SigmoidFunction();
    ~SigmoidFunction();

    double applyFunction(double x);
    double applyDerivativeFunction(double x);
};
#endif