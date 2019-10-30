#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Matrix/Matrix.hpp"

#endif

class ActivationFunction {
    public: 
        Matrix* applyFunction(Matrix* matrix) {
            matrix->mForEach(&this->applyFunction);
            return matrix;
        }

        Matrix* applyDerivativeFunction(Matrix* matrix) {
            matrix->mForEach(&this->applyDerivativeFunction);
            return matrix;
        }

        virtual double applyFunction(double x) = 0;
        virtual double applyDerivativeFunction(double x) = 0;
};