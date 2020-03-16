#include "LayeredNetwork/ActivationFunctions/LeakyReluFunction.hpp"

LeakyReluFunction::LeakyReluFunction() { }
LeakyReluFunction::~LeakyReluFunction() { }

double LeakyReluFunction::applyFunction(double x) {
    return x < 0 ? 0.01 * x : x;
}

double LeakyReluFunction::applyDerivativeFunction(double x) {
    return x < 0 ? 0.01 : 1.0;
}