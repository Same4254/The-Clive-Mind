#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../Matrix/Matrix.hpp"

#endif

class ActivationFunction {
    public: 
        Matrix* applyFunction(Matrix* matrix, Matrix* result) {
            for(int i = 0; i < matrix->getLength(); i++)
                result->getData()[i] = applyFunction(matrix->getData()[i]);
            return result;
        }

        Matrix* applyFunction(Matrix* matrix) {
            for(int i = 0; i < matrix->getLength(); i++)
                matrix->getData()[i] = applyFunction(matrix->getData()[i]);
            return matrix;
        }

        Matrix* applyDerivativeFunction(Matrix* matrix) {
            for(int i = 0; i < matrix->getLength(); i++)
                matrix->getData()[i] = applyDerivativeFunction(matrix->getData()[i]);
            return matrix;
        }

        Matrix* applyDerivativeFunction(Matrix* matrix, Matrix* result) {
            for(int i = 0; i < matrix->getLength(); i++)
                result->getData()[i] = applyDerivativeFunction(matrix->getData()[i]);
            return result;
        }

        virtual double applyFunction(double x) = 0;
        virtual double applyDerivativeFunction(double x) = 0;
};