#ifndef SOFTPLUS_FUNCTION_HPP
#define SOFTPLUS_FUNCTION_HPP

#include "LayeredNetwork/ActivationFunctions/ActivationFunction.hpp"

class SoftplusFunction: public ActivationFunction {
public:
    SoftplusFunction();
    ~SoftplusFunction();

    double applyFunction(double x);
    double applyDerivativeFunction(double x);
};
#endif