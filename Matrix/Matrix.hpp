#include <stdio.h>
#include <iostream>
#include <cmath>
#include <thread>

/**
 *  TODO:
 *      Inline modifier may be beneficial for the set and at methods....
 *      Flipping the matrix is something that should be fully implemented
 * 
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
        bool flipped;

        double atNoPadding(int row, int col) {
            if(row < 0)
                throw std::invalid_argument("Negative Row Index");
            else if(row >= this->nRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw std::invalid_argument("Col Index Overflow");

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
                throw std::invalid_argument("Negative Row Index");
            else if(row >= this->nRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw std::invalid_argument("Col Index Overflow");

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
        /*
        *   The Matrix constructor will store the given data pointer with the given dimensions.
        *   The total length of the data pointer should be row * col, and the values for this pointer should be initialized.
        *   This will start with no padding and untransposed.
        */
        Matrix(double* data, int nRows, int nCols) {
            if(nRows < 0 || nCols < 0)
                throw std::invalid_argument("Cannot have negative number of rows or columns");

            this->data = data;
            this->nRows = nRows;
            this->nCols = nCols;
            this->length = nRows * nCols;
            this->padding = 0;
            this->paddedNRows = nRows;
            this->paddedNCols = nCols;
            this->transposed = false;
            this->flipped = false;
        }

        /*
        *   Initializes a matrix with dimensions nRows x nCols.
        *   Values are initialized to 0, no padding and untransposed.
        */
        Matrix(int nRows, int nCols) : Matrix((double*) calloc(nRows * nCols, sizeof(double)), nRows, nCols) {
        
        }

        /*
        *   Initializes a matrix with dimensions nRows x nCols.
        *   Values are initialized to val, no padding and untransposed.
        */
        Matrix(int nRows, int nCols, double val) : Matrix(nRows, nCols) {
            for(int i = 0; i < length; i++) {
                data[i] = val;
            }
        }

        /*
        *   Initializes a matrix with dimensions nRows x nCols.
        *   Values are initialized to a random double between min and max, no padding and untransposed.
        */
        Matrix(int nRows, int nCols, double min, double max) : Matrix(nRows, nCols) {
            for(int i = 0; i < length; i++)
                data[i] = (max - min) * ((double) rand() / (double) RAND_MAX) + min;
        }

        /*
        *   This will construct a new matrix that is of the same size of the given matrix.
        *   The value of the data, padding, and transpose information will all be copied.
        */
        Matrix(Matrix* other) : Matrix(other->nRows, other->nCols) { 
            for(int i = 0; i < other->length; i++)
                this->data[i] = other->data[i];

            this->padding = other->padding;
            this->paddedNRows = other->paddedNRows;
            this->paddedNCols = other->paddedNCols;

            this->transposed = other->transposed;
        }

        /*
        *   Frees the data pointer
        */
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

        /*
        *   This function will make a direct copy of the given matrix "other".
        *   For the data pointer, the values will be copied. They will NOT point to the same place.
        *   The result matrix must have the same number of rows and columns.
        * 
        *   Return this matrix.
        */
        Matrix* copy(Matrix* other) {
            if(this->nRows != other->nRows || this->nCols != other->nCols)
                throw std::invalid_argument("Unequal Copy Result Size");

            for(int i = 0; i < other->length; i++)
                this->data[i] = other->data[i];

            return this;
        }

        /*
        *   Retrieves the value of the data at the specified 2D position.
        *   This will check if the row and column indecies are within the bounds of the matrix.
        *   In addition, this will take into account whether this matrix is transposed, if it is
        *       this will simply change the way the 1D index is calculated and return the value.
        *   If the value is within the bounds of the padding, a 0.0 will be returned.
        */
        double at(int row, int col) {
            if(row < 0)
                throw std::invalid_argument("Negative Row Index");
            else if(row >= this->paddedNRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->paddedNCols)
                throw std::invalid_argument("Col Index Overflow");

            if(row - padding < 0 || row - padding >= this->nRows)
                return 0.0;
            
            if(col - padding < 0 || col - padding >= this->nCols)
                return 0.0;

            //Row-Major vs Col-Major
            return data[((row - padding) * (transposed ? nRows : nCols)) + col - padding];
        }

        /*
        *   Sets the value of the data at the specified 2D position.
        *   This will check if the row and column indecies are within the bounds of the matrix.
        *   In addition, this will take into account whether this matrix is transposed, if it is
        *       this will simply change the way the 1D index is calculated and set the value.
        *   If the value is within the bounds of the padding, this will throw an exception -> cannot set values that are padded.
        * 
        *   Returns this result matrix.
        */
        Matrix* set(int row, int col, double value) {
            if(row < 0)
                throw std::invalid_argument("Negative Row Index");
            else if(row >= this->paddedNRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->paddedNCols)
                throw std::invalid_argument("Col Index Overflow");

            if(col < padding || col - padding >= this->nCols || row < padding || row - padding >= this->nRows)
                throw std::invalid_argument("Cannot Set padded value");

            //Row-Major vs Col-Major
            this->data[((row - padding) * (transposed ? nRows : nCols)) + col - padding] = value;

            return this;
        }

        /*
        *   Computes the vector magnitude. Either the number of rows or the number of columns must be 1 (it has to be a vector....)
        */
        double vectorMagnitude() {
            if(nRows != 1 && nCols != 1) {
                throw std::invalid_argument("Cannot compute magnitude of a matrix");
            }

            double sum = 0.0;

            for(int i = 0; i < length; i++) {
                sum += pow(data[i], 2.0);
            }

            return sqrt(sum);
        }

        /*
        *   Matrix addition. Adds together all of the elements of this and other matricies and store the result in the result matrix.
        *   All 3 matricies must have the same dimensions.
        * 
        *   Returns the result matrix.
        */
        Matrix* add(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw std::invalid_argument("Unequal Matrix Size When Adding");

            int minPadding = std::min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw std::invalid_argument("Invalid Padding In Result Matrix When Adding");

            //This needs to run fast, so this will not call the at or set functions. Everything needs to be done here.
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

            return result;
        }

        /*
        *   Same as the addition function except the matrix calling this function will also be the result matrix
        * 
        *   Returns this matrix.
        */
        Matrix* mAdd(Matrix* other) {
            return add(other, this);
        }

        /*
        *   Same as the addition function except this is subtraction.
        * 
        *   Returns the result matrix.
        */
        Matrix* subtract(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw std::invalid_argument("Unequal Matrix Size When Subtracting");

            int minPadding = std::min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw std::invalid_argument("Invalid Padding In Result Matrix When Subtracting");

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

            return result;
        }

        /*
        *   Same as the subtract function except the matrix calling this function will also be the result matrix.
        * 
        *   Returns this matrix.
        */
        Matrix* mSubtract(Matrix* other) {
            return subtract(other, this);
        }

        /*
        *   This will multiply all of the elements of the given matricies and store the result in the result matrix.
        *      The dimensions of all matricies involved must have the same dimensions.
        * 
        *   Returns the result matrix.
        */
        Matrix* elementProduct(Matrix* other, Matrix* result) {
            if(other->paddedNCols != this->paddedNCols || result->paddedNCols != this->paddedNCols || 
               other->paddedNRows != this->paddedNRows || result->paddedNRows != this->paddedNRows)
                throw std::invalid_argument("Unequal Matrix Size When Multiplying");

            int minPadding = std::min(this->padding, other->padding);

            if(result->padding > minPadding)
                throw std::invalid_argument("Invalid Padding In Result Matrix When Multiplying");

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

            return result;
        }

        double elementWiseProduct(Matrix* other, int startRow, int startCol, int nRows, int nCols) {
            double sum = 0.0;

            for(int row = startRow; row < nRows + startRow; row++) {
            for(int col = startCol; col < nCols + startCol; col++) {
                // sum += data[(row * this->nCols) + col] * 
                //        other->data[((row - startRow) * other->nCols) + (col - startCol)];

                sum += (flipped ? data[((this->nRows - row - 1) * this->nCols) + (this->nCols - col - 1)] : data[(row * this->nCols) + col]) * 
                       (other->flipped ? other->data[(((other->nRows - row - 1) + startRow) * other->nCols) + ((other->nCols - col - 1) + startCol)] : other->data[((row - startRow) * other->nCols) + (col - startCol)]);

                // sum += (transposed ? data[(col * this->nRows) + row] : data[(row * this->nCols) + col]) * 
                //         (other->transposed ? other->data[((col - startCol) * other->nRows) + (row - startRow)] : other->data[((row - startRow) * other->nCols) + (col - startCol)]);
            }}

            return sum;
        }

        /*
        *   This will simply scale all of the values of this matrix by the given scalar and store the result in the result matrix.
        *   These two matricies must be the same size.
        * 
        *   Returns the result matrix.
        */
        Matrix* scale(double scalar, Matrix* result) {
            if(this->nRows != result->nRows || this->nCols != result->nCols)
                throw std::invalid_argument("Invalid Result Size When Scaling");

            for(int i = 0; i < length; i++) 
                result->data[i] = this->data[i] * scalar;
            return result;
        }

        /*
        *   Scales the values of this matrix by the scalar and stores the result.
        *   Returns this matrix.
        */
        Matrix* mScale(double scalar) {
            return scale(scalar, this);
        }

        /*
        *   Matrix multiplication. 
        * 
        *   Required dimensions: a x b and b x c
        *   The two matricies being multiplied cannot be the matrix as the result matrix.
        * 
        *   Returns the result matrix.
        */
        Matrix* multiply(Matrix* other, Matrix* result) {
            if(result == this || result == other)
                throw std::invalid_argument("Can't store the result of multiplication in the same matrix");

            if(this->paddedNCols != other->paddedNRows)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->paddedNRows != this->paddedNRows || result->paddedNCols != other->paddedNCols)
                throw std::invalid_argument("Invalid result matrix size to multiply");

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

            return result;
        }

        /*
        *   Threaded Matrix multiplication. 
        * 
        *   Required dimensions: a x b and b x c
        *   The two matricies being multiplied cannot be the matrix as the result matrix.
        * 
        *   This will split the work between multiple threads.
        *   Only use this for very large matricies, otherwise the work of setting up threads is greater
        *       than the work of just doing it in one.
        * 
        *   Returns the result matrix.
        */
        Matrix* threadMultiply(Matrix* other, Matrix* result) {
            int amountOFThreads = 2;
            std::thread threads[amountOFThreads];

            int row = 0;
            int amountOfRows = nRows / amountOFThreads;
            for(int i = 0; i < amountOFThreads; i++) {
                if(i == amountOFThreads - 1)
                    threads[i] = std::thread(&Matrix::slaveMultiply, this, other, result, row, nRows);
                else
                    threads[i] = std::thread(&Matrix::slaveMultiply, this, other, result, row, row + amountOfRows);

                row += amountOfRows;
            }

            for(int i = 0; i < amountOFThreads; i++)
                threads[i].join();

            return result;
        }

        /*
        *   Matrix transposition. This fucntion goes through the work of copying the data
        *       into the result matrix.
        *  
        *   The number of rows in this matrix must equal the number of columns in the result matrix.
        *   The number of columns in this matrix must equal the number of rows in the result matrix.
        */
        Matrix* transpose(Matrix* result) {
            if(this == result)
                return mTranspose();

            if(result->nRows != this->nCols || result->nCols != this->nRows)
                throw std::invalid_argument("Invalid Result Matrix Size to Transpose");
            
            for(int row = 0; row < this->nRows; row++) {
            for(int col = 0; col < this->nCols; col++) {
                result->setNoPadding(col, row, this->atNoPadding(row, col));
            }}

            return result;
        }

        /*
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
        *   This is similar to padding but on the inside
        */
        // Matrix* dialate(Matrix* output, int amount) {
        //     if(output->getNRows() != (3 * (amount - 1)) + nRows || output->getNCols() != (3 * (amount - 1)) + nCols)
        //         throw std::invalid_argument("Invalid output size to dialate");

        //     int index = 0;
        //     for(int row = 0; row < output->getNRows(); row += amount) {
        //     for(int col = 0; col < output->getNCols(); col += amount) {
        //         output->set(row, col, data[index]);

        //         index++;
        //     }}

        //     return output;
        // }

        Matrix* dialatePad(Matrix* output, int dialate, int pad) {
            if(output->getNRows() != ((nRows - 1) * (dialate - 1)) + nRows + (2 * pad) || output->getNCols() != ((nRows - 1) * (dialate - 1)) + nCols + (2 * pad))
                throw std::invalid_argument("Invalid output size to dialate");

            int index = 0;
            for(int row = pad; row < output->getNRows() - pad; row += dialate) {
            for(int col = pad; col < output->getNCols() - pad; col += dialate) {
                output->set(row, col, data[index]);

                index++;
            }}

            return output;
        }

        Matrix* mFlip() {
            flipped = !flipped;

            if(transposed && flipped)
                throw std::logic_error("Cannot be both flipped and transposed");

            return this;
        }

        // Matrix* flip(Matrix* output) {
        //     if(nRows != output->getNRows() || nCols != output->getNCols())
        //         throw std::invalid_argument("Invalid output size for flipping matrix");
        // }

        /*
        *   This function is different from the regular transpose function.
        *   Instead of copying data, this will swap the number of rows and columns
        *       to change the way data is accessed in the at and set fucntions.
        *   There is a transpose flag that will be flipped as well.
        */
        Matrix* mTranspose() {
            int temp = this->nRows;
            this->nRows = this->nCols;
            this->nCols = temp;

            this->paddedNRows = (2 * padding) + this->nRows;
            this->paddedNCols = (2 * padding) + this->nCols;

            this->transposed = !this->transposed;

            return this;
        }

        /*
        *   This will take a function pointer that takes a double and returns a double. This function will take in the different elements of 
        *       this matrix, apply the function and store the result in the result matrix.
        * 
        *   The two matricies must have the same dimensions.
        * 
        *   Returns the result matrix.
        */
        Matrix* forEach(double (*function)(double), Matrix* result) {
            if(this->padding != result->padding)
                throw std::invalid_argument("Invalid padding for a for each function");
            if(this->nRows != result->nRows || this->nCols != result->nCols)
                throw std::invalid_argument("Invalid number of rows and cols for a for each");

            for(int i = 0; i < length; i++)
                result->data[i] = function(this->data[i]);

            return result;
        }

        Matrix* mForEach(double (*function)(double)) {
            return forEach(function, this);
        }

        /*
        *   Setting the padding of the matrix means adding an outter layer of 0s that encapsulate the data.
        *   This is done atomically without actually adding any 0s to the data.
        *   
        *   x -> data
        *   padding = 1
        * 
        *   0 0 0 0
        *   0 x x 0
        *   0 0 0 0
        */
        Matrix* setPadding(int padding) {
            if(padding < 0)
                throw std::invalid_argument("Cannot Set Negative Padding");

            this->padding = padding;
            this->paddedNRows = nRows + (2 * padding);
            this->paddedNCols = nCols + (2 * padding);

            return this;
        }

        /*
        *   This will set all of the data values to 0
        */
        Matrix* clear() { 
            for(int i = 0; i < length; i++)
                this->data[i] = 0.0;
            return this;
        }

        /*
        *   Sums together the values of this matrix
        */
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
        int getPadding() { return padding; }

        void print() {
            for(int row = 0; row < nRows + (2 * padding); row++) {
                std::cout << "[";
                for(int col = 0; col < nCols + (2 * padding); col++) {
                    if(col != nCols + (2 * padding) - 1)
                        std::cout << at(row, col) << ", ";
                    else
                        std::cout << at(row, col);
                }
                std::cout << "]" << std::endl;
            }
        }
};