#include "LayeredNetwork/ReluFunction.hpp"

ReluFunction::ReluFunction() {
        
}

ReluFunction::~ReluFunction() {
    
}

double ReluFunction::applyFunction(double x) {
    return std::max(0.0, x);
}

double ReluFunction::applyDerivativeFunction(double x) {
    return x > 0 ? 1 : 0;
}