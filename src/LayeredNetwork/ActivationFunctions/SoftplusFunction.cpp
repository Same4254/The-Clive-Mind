#include "LayeredNetwork/ActivationFunctions/SoftplusFunction.hpp"

SoftplusFunction::SoftplusFunction() { }
SoftplusFunction::~SoftplusFunction() { }

double SoftplusFunction::applyFunction(double x) {
    return std::log(1.0 + exp(x));
}

double SoftplusFunction::applyDerivativeFunction(double x) {
    return 1.0 / (1.0 + exp(-x));
}