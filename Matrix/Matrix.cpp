#include <stdio.h>
#include <iostream>
#include <cmath>
#include <thread>

using namespace std;

/**
 *  The goal of the matrix class is to abstract away the matrix operations.
 *  The Matrix class also allows for preallocated matricies to take in the result of another matrix operation.
 *      This is beneficial to not create matricies on the fly over and over again and fill up memory.
 *      Every operation will have a place in memory if the result cannot be put into a matrix that called it.
 * 
 *  In addition, it provides 0 padding at no extra cost (and it can be changed).
 *      - Assume zero padded values will always be zero (cannot be changed)
 *      - Do NOT allocate memory for all of the zeros
 *      - From the front end, it should appear as though the matrix contains these zeros.
 *          Getting (0, 0) on a 1 padded matrix should always return 0.
 *      - Check if the asked coordinate is inside the padded area.
 *          If in a padded area, return 0;
 *          Otherwise translate the coordinate to get the value inside the actual data.
 *          
 *          (1, 1) in a 1 padded matrix is (0, 0) in the actual data.
 */
class Matrix {
    private:
        double* data;
        
        int nRows, nCols;
        int paddedNRows, paddedNCols;

        int length;
        int padding;

        bool transposed;

        double atNoPadding(int row, int col) {
            if(row < 0)
                throw invalid_argument("Negative Row Index");
            else if(row >= this->nRows)
                throw invalid_argument("Row Index Overflow");

            if(col < 0)
                throw invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw invalid_argument("Col Index Overflow");

            if(transposed)
                return data[(col * nRows) + row];
            else
                return data[(row * nCols) + col];
        }

        double unsafeAtRowMajor(int row, int col) {
            return data[(row * nCols) + col];
        }

        double unsafeAtColMajor(int row, int col) {
            return data[(col * nRows) + row];
        }

        void setNoPadding(int row, int col, double value) {
            if(row < 0)
                throw invalid_argument("Negative Row Index");
            else if(row >= this->nRows)
                throw invalid_argument("Row Index Overflow");

            if(col < 0)
                throw invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw invalid_argument("Col Index Overflow");

            if(transposed)
                data[(col * nRows) + row] = value;
            else
                data[(row * nCols) + col] = value;
        }

        void unsafeSet(int row, int col, double value) {
            data[(row * nCols) + col] = value;
        }

        Matrix* slaveMultiply(Matrix* other, Matrix* result, int startRow, int endRow) {
            if(result->transposed) {
                for(int i = startRow; i < endRow; i++) {
                for(int j = 0; j < other->paddedNCols; j++) {
                    double sum = 0.0;

                    for(int k = 0; k < this->paddedNCols; k++) {
                        sum += (transposed ? data[(k * nRows) + i] : data[(i * nCols) + k])
                                                        * (other->transposed ? other->data[(j * other->nRows) + k] : other->data[(k * other->nCols) + j]);
                    }

                    result->data[(j * result->nRows) + i] = sum;
                }}
            } else {
                for(int i = startRow; i < endRow; i++) {
                for(int j = 0; j < other->paddedNCols; j++) {
                    double sum = 0.0;

                    for(int k = 0; k < this->paddedNCols; k++) {
                        sum += (transposed ? data[(k * nRows) + i] : data[(i * nCols) + k])
                                                        * (other->transposed ? other->data[(j * other->nRows) + k] : other->data[(k * other->nCols) + j]);
                    }

                    result->data[(i * result->nCols) + j] = sum;
                }}
            }

            return result;
        }

    public:
        Matrix(double* data, int nRows, int nCols) {
            this->data = data;
            this->nRows = nRows;
            this->nCols = nCols;
            this->length = nRows * nCols;
            this->padding = 0;
            this->paddedNRows = nRows;
            this->paddedNCols = nCols;
            this->transposed = false;
        }

        Matrix(int nRows, int nCols) : Matrix((double*) calloc(nRows * nCols, sizeof(double)), nRows, nCols) {
        
        }

        Matrix(int nRows, int nCols, double val) : Matrix(nRows, nCols) {
            for(int i = 0; i < length; i++) {
                data[i] = val;
            }
        }

        Matrix(int nRows, int nCols, double min, double max) : Matrix(nRows, nCols) {
            for(int i = 0; i < length; i++)
                data[i] = (max - min) * ((double) rand() / (double) RAND_MAX) + min;
        }

        Matrix(Matrix* other) : Matrix(other->nRows, other->nCols) { 
            for(int i = 0; i < other->length; i++) {
                this->data[i] = other->data[i];
            }
        }

        ~Matrix() { 
            free(data);
        }

        /*
        *   This will swap the data pointer
        *   ONLY use this for continuous hotswapping of data of the same size
        *   This will NOT free up the old pointer
        *   This will NOT adjust the nRows and nCols
        */
        Matrix* setDataUNSAFE(double* data) {
            this->data = data;

            return this;
        }

        Matrix* copy(Matrix* other) {
            if(this->nRows != other->nRows || this->nCols != other->nCols)
                throw invalid_argument("Unequal Copy");

            for(int i = 0; i < other->length; i++) {
                this->data[i] = other->data[i];
            }

            return this;
        }

        double at(int row, int col) {
            if(row < 0)
                throw invalid_argument("Negative Row Index");
            else if(row >= this->nRows + (2 * padding))
                throw invalid_argument("Row Index Overflow");

            if(col < 0)
                throw invalid_argument("Negative Col Index");
            else if(col >= this->nCols + (2 * padding))
                throw invalid_argument("Col Index Overflow");

            if(row - padding < 0 || row - padding >= this->nRows)
                return 0.0;
            
            if(col - padding < 0 || col - padding >= this->nCols)
                return 0.0;

            if(transposed)
                return data[((col - padding) * nRows) + row - padding];
            else
                return data[((row - padding) * nCols) + col - padding];
        }

        void set(int row, int col, double value) {
            if(row < 0)
                throw invalid_argument("Negative Row Index");
            else if(row >= this->paddedNRows)
                throw invalid_argument("Row Index Overflow");

            if(col < 0)
                throw invalid_argument("Negative Col Index");
            else if(col >= this->paddedNCols)
                throw invalid_argument("Col Index Overflow");

            if(col < padding || col - padding >= this->nCols)
                throw invalid_argument("Cannot Set padded value");

            if(transposed)
                data[((col - padding) * nRows) + row - padding] = value;
            else
                data[((row - padding) * nCols) + col - padding] = value;

            // this->data[((row - padding) * nCols) + col - padding] = value;
        }

        Matrix* add(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw invalid_argument("Unequal Matrix Size When Adding");

            int minPadding = min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw invalid_argument("Invalid Padding In Result Matrix When Adding");

            // for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
            // for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
            //     // result->set(row, col, this->at(row, col) + other->at(row, col));
            //     result->data[(row * result->nCols) + col] = data[(row * nCols) + col] + other->data[(row * other->nCols) + col];
            // }}

            if(result->transposed) {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(col * nRows) + row] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        + (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            } else {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(row * nCols) + col] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        + (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            }

            // for(int i = 0; i < this->length; i += nCols) {
            // for(int j = 0; j < nCols; j++) {
            //     result->data[i + j] = this->data[i + j] + other->data[i + j];
            // }}

            return result;
        }

        Matrix* mAdd(Matrix* other) {
            return add(other, this);
        }

        Matrix* subtract(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw invalid_argument("Unequal Matrix Size When Subtracting");

            int minPadding = min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw invalid_argument("Invalid Padding In Result Matrix When Subtracting");

            if(result->transposed) {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(col * nRows) + row] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        - (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            } else {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(row * nCols) + col] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        - (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            }

            // for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
            // for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
            //     result->set(row, col, this->at(row, col) - other->at(row, col));
            // }}

            return result;
        }

        Matrix* mSubtract(Matrix* other) {
            return subtract(other, this);
        }

        Matrix* elementProduct(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw invalid_argument("Unequal Matrix Size When Multiplying");

            int minPadding = min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw invalid_argument("Invalid Padding In Result Matrix When Multiplying");

            if(result->transposed) {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(col * nRows) + row] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        * (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            } else {
                for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
                for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
                    result->data[(row * nCols) + col] = (transposed ? data[(col * nRows) + row] : data[(row * nCols) + col])
                                                        * (other->transposed ? other->data[(col * nRows) + row] : other->data[(row * nCols) + col]);
                }}
            }

            // for(int row = result->padding; row < result->paddedNRows - result->padding; row++) {
            // for(int col = result->padding; col < result->paddedNCols - result->padding; col++) {
            //     result->set(row, col, this->at(row, col) * other->at(row, col));
            // }}

            return result;
        }

        Matrix* scale(double scalar, Matrix* result) {
            if(this->nRows != result->nRows || this->nCols != result->nCols)
                throw invalid_argument("Invalid Result Size When Scaling");

            for(int i = 0; i < length; i++) 
                result->data[i] = this->data[i] * scalar;
            return result;
        }

        Matrix* mScale(double scalar) {
            return scale(scalar, this);
        }

        //Transposed and padding
        Matrix* multiply(Matrix* other, Matrix* result) {
            if(result == this || result == this)
                throw invalid_argument("Can't store the result of multiplication in the same matrix");

            if(this->paddedNCols != other->paddedNRows)
                throw invalid_argument("Invalid matrix size to multiply");
            
            if(result->paddedNRows != this->paddedNRows || result->paddedNCols != other->paddedNCols)
                throw invalid_argument("Invalid result matrix size to multiply");

            if(result->transposed) {
                for(int i = 0; i < this->paddedNRows; i++) {
                for(int j = 0; j < other->paddedNCols; j++) {
                    double sum = 0.0;

                    for(int k = 0; k < this->paddedNCols; k++) {
                        sum += (transposed ? data[(k * nRows) + i] : data[(i * nCols) + k])
                                                        * (other->transposed ? other->data[(j * other->nRows) + k] : other->data[(k * other->nCols) + j]);
                    }

                    result->data[(j * result->nRows) + i] = sum;
                }}
            } else {
                for(int i = 0; i < this->paddedNRows; i++) {
                for(int j = 0; j < other->paddedNCols; j++) {
                    double sum = 0.0;

                    for(int k = 0; k < this->paddedNCols; k++) {
                        sum += (transposed ? data[(k * nRows) + i] : data[(i * nCols) + k])
                                                        * (other->transposed ? other->data[(j * other->nRows) + k] : other->data[(k * other->nCols) + j]);
                    }

                    result->data[(i * result->nCols) + j] = sum;
                }}
            }

            // for(int i = minPadding; i < this->paddedNRows - minPadding; i++) {
            // for(int j = minPadding; j < other->paddedNCols - minPadding; j++) {
            //     double sum = 0.0;

            //     for(int k = minPadding; k < this->paddedNCols - minPadding; k++) {
            //         sum += this->at(i, k) * other->at(k, j);
            //     }

            //     result->set(i, j, sum);
            // }}

            return result;
        }

        Matrix* threadMultiply(Matrix* other, Matrix* result) {
            int amountOFThreads = 2;
            thread threads[amountOFThreads];

            int row = 0;
            int amountOfRows = nRows / amountOFThreads;
            for(int i = 0; i < amountOFThreads; i++) {
                if(i == amountOFThreads - 1)
                    threads[i] = thread(&Matrix::slaveMultiply, this, other, result, row, nRows);
                else
                    threads[i] = thread(&Matrix::slaveMultiply, this, other, result, row, row + amountOfRows);

                row += amountOfRows;
            }

            for(int i = 0; i < amountOFThreads; i++)
                threads[i].join();

            return result;
        }

        Matrix* transpose(Matrix* result) {
            if(this == result)
                return mTranspose();

            if(result->nRows != this->nCols || result->nCols != this->nRows)
                throw invalid_argument("Invalid Result Matrix Size to Transpose");
            
            for(int row = 0; row < this->nRows; row++) {
            for(int col = 0; col < this->nCols; col++) {
                result->setNoPadding(col, row, this->atNoPadding(row, col));
            }}

            return result;
        }

        Matrix* mTranspose() {
            int temp = this->nRows;
            this->nRows = this->nCols;
            this->nCols = temp;

            this->paddedNRows = (2 * padding) + this->nRows;
            this->paddedNCols = (2 * padding) + this->nCols;

            this->transposed = !this->transposed;

            return this;
        }

        Matrix* forEach(double (*function)(double), Matrix* result) {
            if(this->padding != result->padding)
                throw invalid_argument("Invalid padding for a for each function");
            if(this->nRows != result->nRows || this->nCols != result->nCols)
                throw invalid_argument("Invalid number of rows and cols for a for each");

            for(int i = 0; i < length; i++)
                result->data[i] = function(this->data[i]);

            return result;
        }

        Matrix* setPadding(int padding) {
            if(padding < 0)
                throw invalid_argument("Cannot Set Negative Padding");

            this->padding = padding;
            this->paddedNRows = nRows + (2 * padding);
            this->paddedNCols = nCols + (2 * padding);

            return this;
        }

        Matrix* clear() { 
            for(int i = 0; i < length; i++)
                this->data[i] = 0.0;
            return this;
        }

        double sumElements() { 
            double result = 0.0;
            for(int i = 0; i < length; i++)
                result += this->data[i];

            return result;
        }

        double* getData() { return data; }
        int getNRows() { return paddedNRows; }
        int getNCols() { return paddedNCols; }
        int getLength() { return length; }

        void print() {
            for(int row = 0; row < nRows + (2 * padding); row++) {
                cout << "[";
                for(int col = 0; col < nCols + (2 * padding); col++) {
                    if(col != nCols + (2 * padding) - 1)
                        cout << at(row, col) << ", ";
                    else
                        cout << at(row, col);
                }
                cout << "]" << endl;
            }
        }
};