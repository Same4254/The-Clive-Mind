#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdio.h>
#include <iostream>
#include <cmath>

class Matrix {
private:
    double* data;
    double** data2D;
    
    int nRows, nCols;
    int length;

    /**
     * Updates the 2 dimensonal structure of the data to the 
     */
    void updateData2D();

public:

    /**
    *   The Matrix constructor will store the given data pointer with the given dimensions.
    *   The total length of the data pointer should be row * col, and the values for this pointer should be initialized.
    * 
    *   The dimensions of the matrix are not mutable. They cannot be changed after the creation of the matrix 
    * 
    *   @param data -> The pointer to store the data in
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    */
    Matrix(double* data, int nRows, int nCols);

    /**
    *   Initializes a matrix with dimensions nRows x nCols.
    *   Values are initialized to 0
    * 
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    */
    Matrix(int nRows, int nCols);

    /**
    *   Initializes a matrix with dimensions nRows x nCols.
    *   Values are initialized to val
    * 
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    *   @param val -> the value to set for all data in this matrix
    */
    Matrix(int nRows, int nCols, double val);

    /**
    *   Initializes a matrix with dimensions nRows x nCols.
    *   Values are initialized to a random double between min and max
    * 
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    *   @param min -> The minimum bound of the random number generator
    *   @param max -> The maximum bound of the random number generator
    */
    Matrix(int nRows, int nCols, double min, double max);

    /**
    *   Initializes a matrix with dimensions nRows x nCols, and stores values at the provided memory address.
    *   Values are initialized to a random double between min and max
    * 
    *   @param data -> Pointer to the data
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    *   @param min -> The minimum bound of the random number generator
    *   @param max -> The maximum bound of the random number generator
    */
    Matrix(double* data, int nRows, int nCols, double min, double max);

    /**
    *   Creates an exact copy of the priovided matrix 
    * 
    *   @param other -> Matrix to copy
    */
    Matrix(Matrix* other);

    /**
    *   Be FREEEEE!
    */
    ~Matrix();

    /**
    *   Use with caution. The idea of this function is to allow quick hot-swapping
    *       of data not created by this matrix. This will not free the current pointer, nor will it adjust dimensional information.
    * 
    *   @param data -> Pointer to read and write data to
    *   @return this matrix
    */
    Matrix* setData(double* data);

    /**
    *   Use with caution. The idea of this function is to allow quick hot-swapping
    *       of data not created by this matrix. This will not free the current pointer.
    * 
    *   @param data -> Pointer to read and write data to
    *   @param nRows -> The amount of rows in the 2-dimensional representation of the matrix
    *   @param nCols -> The amount of columns in the 2-dimensional representation of the matrix
    *   @return This matrix
    */
    // Matrix* setData(double* data, int nRows, int nCols);

    /**
    *   This function will make a direct copy of the given matrix "other".
    *   For the data pointer, the values will be copied. They will NOT point to the same place.
    *   The two must have the same 1-d data length (not neccessarily the same dimensions)
    * 
    *   @param other -> Matrix to copy
    *   @return This matrix
    */
    Matrix* copyData(Matrix* other);

    /**
    *   Retrieves the value of the data at the specified 2D position.
    *   This will check if the row and column indecies are within the bounds of the matrix.
    * 
    *   @param row -> The row to retrieve the data from
    *   @param col -> The column to retrieve the data from
    *   @return The value at this position
    */
    double at(int row, int col);

    /**
    *   Sets the value of the data at the specified 2D position.
    *   This will check if the row and column indecies are within the bounds of the matrix.
    *   
    *   @param row -> The row to set the data to
    *   @param col -> The column to set the data to
    *   @param value -> The value to store at the row, column position
    *   @return This matrix
    */
    Matrix* set(int row, int col, double value);

    /**
    *   Computes the vector magnitude. Either the number of rows or the number of columns must be 1 (it has to be a vector....)
    * 
    *   @return -> The vector magnitude
    */
    double vectorMagnitude();

    /**
    *   Matrix addition. Adds together all of the elements of this matrix and the other matrix and stores the result in the result matrix.
    *   All 3 matricies must have the same dimensions.
    * 
    *   result = this + other
    * 
    *   @param other -> Matrix to add with
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* add(Matrix* other, Matrix* result);

    /**
    *   Mutable Matrix addition. Adds together the elements of this matrix with the other matrix. The result is stored in this matrix.
    *   Dimensions must be the same
    *   
    *   @param other -> Matrix to add with
    *   @return This matrix
    */
    Matrix* mAdd(Matrix* other);

    /**
    *   Adds the value adder with all elements of this matrix and stores the result in the result matrix.
    *   Dimensions of this matrix and the result matrix must be the same.
    * 
    *   @param adder -> Value to add with the elements in this matrix
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* add(double adder, Matrix* result);

    /**
    *   Adds the value of adder to all the elements of this matrix.
    *  
    *   @param adder -> Value to add to all of the data in this matrix
    *   @return This matrix
    */
    Matrix* mAdd(double adder);

    /**
    *   Matrix subtraction. Subtract all of the elements of this by the other matrix's data and store the result in the result matrix.
    *   All 3 matricies must have the same dimensions.
    * 
    *   result = this - other
    * 
    *   @param other -> Matrix to subtract by
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* subtract(Matrix* other, Matrix* result);

    /**
    *   Mutable Matrix subtraction. Subtract all of the elements of this by the other matrix's data and store the result in this matrix.
    *   This matrix and the other matrix must have the same dimensional size.
    * 
    *   @param other -> Matrix to subtract by
    *   @return This matrix
    */
    Matrix* mSubtract(Matrix* other);

    /**
    *   Raises all elements of this matrix to the power p and stores the result in the result matrix.
    *   This matrix and the other matrix must have the same dimensional size.
    * 
    *   @param p -> power to raise all elements by
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* pow(double p, Matrix* result);

    /**
    *   Multiplies the elements in this matrix with the other matrix, and stores the result in the result matrix.
    *   All 3 matricies must have the same dimensional size.
    * 
    *   @param other -> Matrix to multiply with
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* elementProduct(Matrix* other, Matrix* result);

    /**
    *   Mutable element product. Multiplies the elements of this matrix and the other matrix, and stores the result in this matrix.
    * 
    *   @param other -> Matrix to multiply with
    *   @return This matrix
    */
    Matrix* mElementProduct(Matrix* other);

    /**
    *   Divides the elements of this matrix by the elements in the other matrix, and stores the result in the result matrix.
    * 
    *   result = this / other
    * 
    *   @param other -> Matrix to divide by
    *   @param result -> Matrix to store the result in
    *   @return The result matrix
    */
    Matrix* elementDivision(Matrix* other, Matrix* result);

    /**
    *   The segmentedElementWiseProductSum is the sum of the products of the elements of two matricies. 
    * 
    *   | a b c | * | g h i | = ga + bh + ci + dk + el + mf
    *   | d e f |   | k l m |
    * 
    *   However, convolution asks for a smaller matrix to do this on parts of a bigger matrix.
    *   Thus, this function will calculate this sum over the area of the smaller matrix at the location specified.
    *   Thus the large matrix must call this method and provide the small matrix and where it wants the small matrix to go.
    * 
    *   The large matrix must call this, not the small.
    * 
    *   @param other -> The smaller matrix to place "on top" of this matrix and calculate the element product with
    *   @param startRow -> The upper left corner row location (relative to the larger matrix) of where the smaller matrix is to be placed
    *   @param startRow -> The upper left corner column location (relative to the larger matrix) of where the smaller matrix is to be placed
    *   @return the sum of the element product
    */
    double segmentedElementWiseProductSum(Matrix* other, int startRow, int startCol);

    /**
    *   This is the same as the original elementWiseProduct function except it accesses the smaller matrix as if it were turned 180 degrees.
    * 
    *   @param other -> The smaller matrix to place "on top" of this matrix and calculate the element product with
    *   @param startRow -> The upper left corner row location (relative to the larger matrix) of where the smaller matrix is to be placed
    *   @param startRow -> The upper left corner column location (relative to the larger matrix) of where the smaller matrix is to be placed
    *   @return the sum of the flipped element product
    */
    double segmentedFlippedElementWiseProductSum(Matrix* other, int startRow, int startCol);

    /**
    *   This will scale all of the values of this matrix by the given scalar and store the result in the result matrix.
    *   These two matricies must be the same size.
    * 
    *   @param scalar -> Value to scale all elements by
    *   @param result -> The matrix to store the result in
    *   @return The result matrix
    */
    Matrix* scale(double scalar, Matrix* result);

    /**
    *   Mutable Scaling. This will scale all elements of this matrix by the scalar, and store the result in this matrix.
    * 
    *   @param scalar -> Value to scale all elements by
    *   @return This matrix
    */
    Matrix* mScale(double scalar);

    /**
    *   Traditional Matrix multiplication, that stores the result in the result matrix.
    * 
    *   Required dimensions, this: a x b. other: b x c. result: a x c.
    *   The two matricies being multiplied cannot be the matrix as the result matrix (Cannot be the same matrix memory address).
    * 
    *   @param other -> Matrix to multiply with
    *   @param result -> Matrix to store the result in 
    *   @return The result matrix
    */
    Matrix* multiply(Matrix* other, Matrix* result);

    /**
    *   Traditional Matrix multiplication, that stores the result in the result matrix.
    *   However, this will act as though the "other" matrix is meant to be transposed.
    *   
    *   Note: the matrix itself is not really transposed, the idea here is to get the result as if it were, without creating a whole
    *       new matrix to store the transposed version. This will access the matrix in a way that will mimick the result if it were really transposed.
    * 
    *   Required dimensions, this: a x b. other: c x b. result: a x c.
    *   
    *   Note: the result matrix cannot be the same matrix (same memory address) as the two other matricies
    * 
    *   @param other -> Matrix to multiply with
    *   @param result -> Matrix to store the result in 
    *   @return The result matrix
    */
    Matrix* multiplyOtherTransposed(Matrix* other, Matrix* result);

    /**
    *   This will act out matrix multiplication with the "other" matrix (which is mimicked to be transposed).
    *   Then it will add the corresponding value from the adder matrix, all in one go.
    * 
    *   This will eliminate the need for a middle-man matrix of the multiplication and addition
    * 
    *   Dimensional requirements: this: a x b. other: c x b, adder: a x c, result: a x c.
    * 
    *   Note: the result matrix cannot be the same matrix (same memory address) as the two matricies being multiplied
    * 
    *   @param other -> The matrix to multiply this with (used as though it were transposed).
    *   @param adder -> The matrix to add the multiplication result with.
    *   @param result -> Matrix to store the end result in.
    *   @return The result matrix
    */
    Matrix* multiplyOtherTransposedAdded(Matrix* other, Matrix* adder, Matrix* result);

    /**
    *   Traditional Matrix multiplication, that stores the result in the result matrix.
    *   However, this will act as though this matrix is meant to be transposed.
    *   
    *   Note: the matrix itself is not really transposed, the idea here is to get the result as if it were, without creating a whole
    *       new matrix to store the transposed version. This will access the matrix in a way that will mimick the result if it were really transposed.
    * 
    *   Required dimensions, this: b x a. other: b x c. result: a x c.
    * 
    *   Note: the result matrix cannot be the same matrix (same memory address) as the two other matricies
    * 
    *   @param other -> Matrix to multiply with
    *   @param result -> Matrix to store the result in 
    *   @return The result matrix
    */
    Matrix* multiplyInputTransposed(Matrix* other, Matrix* result);

    /**
    *   This will act out matrix multiplication with the "other" matrix.
    *   However, this matrix (the caller) will be used as though it were transposed
    * 
    *   Then it will add the corresponding value from the adder matrix, all in one go.
    * 
    *   This will eliminate the need for a middle-man matrix of the multiplication and addition
    * 
    *   Dimensional requirements: this: b x a. other: c x b, adder: a x c, result: a x c.
    * 
    *   Note: the result matrix cannot be the same matrix (same memory address) as the two matricies being multiplied
    * 
    *   @param other -> The matrix to multiply this with.
    *   @param adder -> The matrix to add the multiplication result with.
    *   @param result -> Matrix to store the end result in.
    *   @return The result matrix
    */
    Matrix* multiplyInputTransposedAdded(Matrix* other, Matrix* adder, Matrix* result);

    /**
    *   Matrix transposition. This will copy the data of this matrix into the result matrix, but transposed.
    * 
    *   Dimensional Requirements: this: a x b. result: b x a.
    *  
    *   @param result -> matrix to store the transposed version of this matrix
    *   @return The result matrix
    */
    Matrix* transpose(Matrix* result);

    /**
    *   Dialation refers to inserting 0s in between the data:
    *   Dialation 1:
    *       xxx
    *       xxx
    *       xxx
    *   
    *   Dialation 2:
    *       x 0 x 0 x
    *       x 0 x 0 x
    *       x 0 x 0 x
    * 
    *   Padding 1:
    *       0 0 0 0 0
    *       0 x x x 0
    *       0 x x x 0
    *       0 x x x 0
    *       0 0 0 0 0 
    * 
    *   @param dialate -> Amount to dialate this matrix by
    *   @param pad -> Amount to pad this matrix by
    *   @return The result matrix
    */
    Matrix* dialatePad(int dialate, int pad, Matrix* result);

    /**
    *   This will take a function pointer that takes a double and returns a double. This function will take each element of 
    *       this matrix, apply the function and store the result in the result matrix.
    * 
    *   The two matricies must have the same dimensions.
    * 
    *   @param function -> Function pointer to apply to the data of this matrix
    *   @param result -> Result matrix to store the result
    *   @return The result matrix
    */
    Matrix* forEach(double (*function)(double), Matrix* result);

    /**
    *   Mutable. This will take a function pointer that takes a double and returns a double. This function will take each element of 
    *       this matrix, apply the function and store the result in this matrix.
    * 
    *   The two matricies must have the same dimensions.
    * 
    *   @param function -> Function pointer to apply to the data of this matrix
    *   @return This matrix
    */
    Matrix* mForEach(double (*function)(double));

    /**
    *   Sets all of the data to 0
    */
    Matrix* clear();

    /**
    *   Sums together the values of this matrix
    */
    double sumElements();

    /**
    *   Returns the greatest value in this matrix
    */ 
    double greatest();

    /**
    *   Returns the 1-dimensional index of the greatest value in this matrix
    */
    int greatestIndex();

    double* getData();
    int getNRows();
    int getNCols();
    int getLength();

    void print();
};
#endif