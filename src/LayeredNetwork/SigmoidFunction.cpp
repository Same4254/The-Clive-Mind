#include "LayeredNetwork/SigmoidFunction.hpp"

SigmoidFunction::SigmoidFunction() {
        
}

double SigmoidFunction::applyFunction(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double SigmoidFunction::applyDerivativeFunction(double x) {
    return applyFunction(x) * (1.0 - applyFunction(x));
}