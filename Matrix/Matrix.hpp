#include <stdio.h>
#include <iostream>
#include <cmath>

class Matrix {
    private:
        double* data;
        
        int nRows, nCols;
        int length;

        double unsafeAtRowMajor(int row, int col) {
            return data[(row * nCols) + col];
        }

        double unsafeAtColMajor(int row, int col) {
            return data[(col * nRows) + row];
        }

        void unsafeSet(int row, int col, double value) {
            data[(row * nCols) + col] = value;
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

        Matrix(double* data, int nRows, int nCols, double min, double max) : Matrix(data, nRows, nCols) {
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

            this->nRows = other->nRows;
            this->nCols = other->nCols;
        }

        /*
        *   Be FREEEEE!
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
        Matrix* setData(double* data) {
            this->data = data;

            return this;
        }

        Matrix* setData(double* data, int nRows, int nCols) {
            this->data = data;
            this->nRows = nRows;
            this->nCols = nCols;
            this->length = nRows * nCols;

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
        */
        double at(int row, int col) {
            if(row < 0)
                throw std::invalid_argument("Negative Row Index");
            else if(row >= this->nRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw std::invalid_argument("Col Index Overflow");

            //Row-Major vs Col-Major
            return data[(row * nCols) + col];
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
            else if(row >= this->nRows)
                throw std::invalid_argument("Row Index Overflow");

            if(col < 0)
                throw std::invalid_argument("Negative Col Index");
            else if(col >= this->nCols)
                throw std::invalid_argument("Col Index Overflow");

            //Row-Major vs Col-Major
            this->data[(row * nCols) + col] = value;

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
                sum += std::pow(data[i], 2.0);
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
            if(other->nCols != this->nCols || result->nCols != this->nCols || 
               other->nRows != this->nRows || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Adding");

            for(int i = 0; i < length; i++)
                result->data[i] = data[i] + other->data[i];

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
        *   Adds the adder value to all of the elements in the matrix
        */
        Matrix* add(double adder, Matrix* result) {
            if(result->nCols != this->nCols || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Adding");

            for(int i = 0; i < length; i++)
                result->data[i] = data[i] + adder;

            return result;
        }

        Matrix* mAdd(double adder) {
            return add(adder, this);
        }

        /*
        *   Same as the addition function except this is subtraction.
        * 
        *   Returns the result matrix.
        */
        Matrix* subtract(Matrix* other, Matrix* result) {
            if(other->nCols != this->nCols || result->nCols != this->nCols || 
               other->nRows != this->nRows || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Subtracting");

            for(int i = 0; i < length; i++)
                result->data[i] = data[i] - other->data[i];

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

        Matrix* pow(double p, Matrix* result) {
            if(result->nCols != this->nCols || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Powing");

            for(int i = 0; i < length; i++)
                result->data[i] = std::pow(data[i], p);

            return result;
        }

        /*
        *   This will multiply all of the elements of the given matricies and store the result in the result matrix.
        *      The dimensions of all matricies involved must have the same dimensions.
        * 
        *   Returns the result matrix.
        */
        Matrix* elementProduct(Matrix* other, Matrix* result) {
            if(other->nCols != this->nCols || result->nCols != this->nCols || 
               other->nRows != this->nRows || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Element Producting");

            for(int i = 0; i < length; i++)
                result->data[i] = data[i] * other->data[i];

            return result;
        }

        Matrix* mElementProduct(Matrix* other) {
            elementProduct(other, this);
        }

        /*
        *   This will divide all of the elements of the given matricies and store the result in the result matrix.
        *      The dimensions of all matricies involved must have the same dimensions.
        * 
        *   Returns the result of dividing the elements of the caller matrix by the elements of the given matrix. Stored in result matrix
        */
        Matrix* elementDivision(Matrix* other, Matrix* result) {
            if(other->nCols != this->nCols || result->nCols != this->nCols || 
               other->nRows != this->nRows || result->nRows != this->nRows)
                throw std::invalid_argument("Unequal Matrix Size When Dividing");

            for(int i = 0; i < length; i++)
                result->data[i] = data[i] / other->data[i];

            return result;
        }

        /*
        *   The elementWiseProduct is the sum of the products of the elements of two matricies. 
        * 
        *   a b c  *  g h i = ga + bh + ci + dk + el + mf
        *   d e f     k l m
        * 
        *   However, convolution asks for a smaller matrix to do this on parts of a bigger matrix.
        *   Thus the large matrix can call this method and provide the small matrix and where it wants
        *       the small matrix to go.
        * 
        *   The large matrix must call this, not the small.
        *   This has no support for mTransposed matricies
        */
        double elementWiseProduct(Matrix* other, int startRow, int startCol, int nRows, int nCols) {
            double sum = 0.0;

            for(int row = startRow; row < nRows + startRow; row++) {
            for(int col = startCol; col < nCols + startCol; col++) {
                sum += data[(row * this->nCols) + col] * other->data[((row - startRow) * other->nCols) + (col - startCol)];
            }}

            return sum;
        }

        /*
        *   This is the same as the original elementWiseProduct function except it accesses the smaller matrix as if it were turned 180 degrees.
        */
        double flippedElementWiseProduct(Matrix* other, int startRow, int startCol, int nRows, int nCols) {
            double sum = 0.0;

            for(int row = startRow; row < nRows + startRow; row++) {
            for(int col = startCol; col < nCols + startCol; col++) {
                sum += data[(row * this->nCols) + col] * other->data[(((other->nRows - row - 1) + startRow) * other->nCols) + ((other->nCols - col - 1) + startCol)];
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

            if(this->nCols != other->nRows)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->nRows != this->nRows || result->nCols != other->nCols)
                throw std::invalid_argument("Invalid result matrix size to multiply");

            for(int i = 0; i < this->nRows; i++) {
            for(int j = 0; j < other->nCols; j++) {
                double sum = 0.0;

                for(int k = 0; k < this->nCols; k++) {
                    sum += data[(i * nCols) + k] * other->data[(k * other->nCols) + j];
                }

                result->data[(i * result->nCols) + j] = sum;
            }}

            return result;
        }

        /*
        *   Multiplies the matricies as if "other" is transposed
        */
        Matrix* multiplyOtherTransposed(Matrix* other, Matrix* result) {
            if(result == this || result == other)
                throw std::invalid_argument("Can't store the result of multiplication in the same matrix");

            if(this->nCols != other->nCols)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->nRows != this->nRows || result->nCols != other->nRows)
                throw std::invalid_argument("Invalid result matrix size to multiply");

            for(int i = 0; i < this->nRows; i++) {
            for(int j = 0; j < other->nRows; j++) {
                double sum = 0.0;

                for(int k = 0; k < this->nCols; k++) {
                    sum += data[(i * nCols) + k] * other->data[(j * other->nCols) + k];
                }

                result->data[(i * result->nCols) + j] = sum;
            }}

            return result;
        }

        /*
        *   Multiplies the matricies as if "other" is transposed, and adds the adder matrix
        */
        Matrix* multiplyOtherTransposedAdded(Matrix* other, Matrix* adder, Matrix* result) {
            if(result == this || result == other)
                throw std::invalid_argument("Can't store the result of multiplication in the same matrix");

            if(adder->nRows != result->nRows || adder->nCols != result->nCols)
                throw std::invalid_argument("Invalid adder size to multiply");

            if(this->nCols != other->nCols)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->nRows != this->nRows || result->nCols != other->nRows)
                throw std::invalid_argument("Invalid result matrix size to multiply");

            for(int i = 0; i < this->nRows; i++) {
            for(int j = 0; j < other->nRows; j++) {
                double sum = 0.0;

                for(int k = 0; k < this->nCols; k++) {
                    sum += data[(i * nCols) + k] * other->data[(j * other->nCols) + k];
                }

                result->data[(i * result->nCols) + j] = sum + adder->data[(i * result->nCols) + j];
            }}

            return result;
        }

        /*
        *   Multiplies the matricies as if the caller matrix is transposed
        */
        Matrix* multiplyInputTransposed(Matrix* other, Matrix* result) {
            if(result == this || result == other)
                throw std::invalid_argument("Can't store the result of multiplication in the same matrix");

            if(this->nRows != other->nRows)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->nRows != this->nCols || result->nCols != other->nCols)
                throw std::invalid_argument("Invalid result matrix size to multiply");

            for(int i = 0; i < this->nCols; i++) {
            for(int j = 0; j < other->nCols; j++) {
                double sum = 0.0;

                for(int k = 0; k < this->nRows; k++) {
                    sum += data[(k * nCols) + i] * other->data[(k * other->nCols) + j];
                }

                result->data[(i * result->nCols) + j] = sum;
            }}

            return result;
        }

        /*
        *   Multiplies the matricies as if the caller matrix is transposed, and adds the values from the adder matrix
        */
        Matrix* multiplyInputTransposedAdded(Matrix* other, Matrix* adder, Matrix* result) {
            if(result == this || result == other)
                throw std::invalid_argument("Can't store the result of multiplication in the same matrix");

            if(adder->nRows != result->nRows || adder->nCols != result->nCols)
                throw std::invalid_argument("Invalid adder size to multiply");

            if(this->nRows != other->nRows)
                throw std::invalid_argument("Invalid matrix size to multiply");
            
            if(result->nRows != this->nCols || result->nCols != other->nCols)
                throw std::invalid_argument("Invalid result matrix size to multiply");

            for(int i = 0; i < this->nCols; i++) {
            for(int j = 0; j < other->nCols; j++) {
                double sum = 0.0;

                for(int k = 0; k < this->nRows; k++) {
                    sum += data[(k * nCols) + i] * other->data[(k * other->nCols) + j];
                }

                result->data[(i * result->nCols) + j] = sum + adder->data[(i * result->nCols) + j];
            }}

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
            if(result->nRows != this->nCols || result->nCols != this->nRows)
                throw std::invalid_argument("Invalid Result Matrix Size to Transpose");
            
            for(int row = 0; row < this->nRows; row++) {
            for(int col = 0; col < this->nCols; col++) {
                result->set(col, row, this->at(row, col));
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
        *   Padding 1:
        *       0 0 0 0 0
        *       0 x x x 0
        *       0 x x x 0
        *       0 x x x 0
        *       0 0 0 0 0 
        */
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

        /*
        *   This will take a function pointer that takes a double and returns a double. This function will take in the different elements of 
        *       this matrix, apply the function and store the result in the result matrix.
        * 
        *   The two matricies must have the same dimensions.
        * 
        *   Returns the result matrix.
        */
        Matrix* forEach(double (*function)(double), Matrix* result) {
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

        double greatest() {
            double g = data[0];

            for(int i = 1; i < length; i++)
                if(data[i] > g)
                    g = data[i];

            return g;
        }

        double greatestIndex() {
            int i = 0;
            for(int j = 1; j < length; j++) 
                if(data[j] > data[i])
                    i = j;
            
            return i;
        }

        double* getData() { return data; }
        int getNRows() { return nRows; }
        int getNCols() { return nCols; }
        int getLength() { return length; }

        void print() {
            for(int row = 0; row < nRows; row++) {
                std::cout << "[";
                for(int col = 0; col < nCols; col++) {
                    if(col != nCols - 1)
                        std::cout << at(row, col) << ", ";
                    else
                        std::cout << at(row, col);
                }
                std::cout << "]" << std::endl;
            }
        }
};