#ifndef ACTIVATION_FUNCTION_HPP
#define ACTIVATION_FUNCTION_HPP

#include "ActivationFunction.hpp"

#endif

class ReluFunction: public ActivationFunction {
    public:
        ReluFunction() {
            
        }

        ~ReluFunction() {
            
        }

        double applyFunction(double x) {
            return std::max(0.0, x);
        }

        double applyDerivativeFunction(double x) {
            return x > 0 ? 1 : 0;
        }
};