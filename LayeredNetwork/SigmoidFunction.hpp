#ifndef ACTIVATION_FUNCTION_HPP
#define ACTIVATION_FUNCTION_HPP

#include "./ActivationFunction.hpp"

#endif

class SigmoidFunction: public ActivationFunction {
    public:
        SigmoidFunction() {
            
        }

        double applyFunction(double x) {
            return 1.0 / (1.0 + exp(-x));
        }

        double applyDerivativeFunction(double x) {
            return applyFunction(x) * (1.0 - applyFunction(x));
        }
};