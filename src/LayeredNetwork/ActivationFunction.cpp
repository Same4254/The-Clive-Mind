#include "LayeredNetwork/ActivationFunction.hpp"

Matrix* ActivationFunction::applyFunction(Matrix* matrix, Matrix* result) {
    for(int i = 0; i < matrix->getLength(); i++)
        result->getData()[i] = applyFunction(matrix->getData()[i]);
    return result;
}

Matrix* ActivationFunction::applyFunction(Matrix* matrix) {
    for(int i = 0; i < matrix->getLength(); i++)
        matrix->getData()[i] = applyFunction(matrix->getData()[i]);
    return matrix;
}

Matrix* ActivationFunction::applyDerivativeFunction(Matrix* matrix, Matrix* result) {
    for(int i = 0; i < matrix->getLength(); i++)
        result->getData()[i] = applyDerivativeFunction(matrix->getData()[i]);
    return result;
}

Matrix* ActivationFunction::applyDerivativeFunction(Matrix* matrix) {
    for(int i = 0; i < matrix->getLength(); i++)
        matrix->getData()[i] = applyDerivativeFunction(matrix->getData()[i]);
    return matrix;
}