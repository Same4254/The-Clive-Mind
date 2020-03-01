#ifndef ACTIVATION_FUNCTION_HPP
#define ACTIVATION_FUNCTION_HPP

#include "Matrix.hpp"

class ActivationFunction {
public: 
    virtual ~ActivationFunction();

    /**
    *   This will apply the function on the elements in the matrix and store the result in the result matrix 
    * 
    *   @param matrix -> matrix with the data to put through the function
    *   @param result -> the result matrix to store the result
    *   @return The result matrix
    */
    Matrix* applyFunction(Matrix* matrix, Matrix* result);

    /**
    *   Mutable. This will apply the function on the elements in the matrix.
    * 
    *   @param matrix -> matrix with the data to be put through the function
    *   @return The same matrix
    */
    Matrix* applyFunction(Matrix* matrix);

    /**
    *   This will apply the derivative function on the elements in the matrix and store the result in the result matrix 
    * 
    *   @param matrix -> matrix with the data to put through the derivative function
    *   @param result -> the result matrix to store the result
    *   @return The result matrix
    */
    Matrix* applyDerivativeFunction(Matrix* matrix, Matrix* result);

    /**
    *   Mutable. This will apply the derivative function on the elements in the matrix.
    * 
    *   @param matrix -> matrix with the data to be put through the derivative function
    *   @return The same matrix
    */
    Matrix* applyDerivativeFunction(Matrix* matrix);

    virtual double applyFunction(double x) = 0;
    virtual double applyDerivativeFunction(double x) = 0;
};
#endif