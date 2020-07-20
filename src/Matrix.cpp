#include "Matrix.hpp"

Matrix::Matrix(double* data, int nRows, int nCols) {
    if(nRows < 0 || nCols < 0)
        throw std::invalid_argument("Cannot have negative number of rows or columns");

    this->data = data;
    this->nRows = nRows;
    this->nCols = nCols;
    this->length = nRows * nCols;

    data2D = (double**) calloc(nRows, sizeof(double*));

    updateData2D();
}

Matrix::Matrix(int nRows, int nCols) : Matrix((double*) calloc(nRows * nCols, sizeof(double)), nRows, nCols) { }

Matrix::Matrix(int nRows, int nCols, double val) : Matrix(nRows, nCols) {
    for(int i = 0; i < length; i++) {
        data[i] = val;
    }
}

Matrix::Matrix(int nRows, int nCols, double min, double max) : Matrix(nRows, nCols) {
    for(int i = 0; i < length; i++)
        data[i] = (max - min) * ((double) rand() / (double) RAND_MAX) + min;
}

Matrix::Matrix(double* data, int nRows, int nCols, double min, double max) : Matrix(data, nRows, nCols) {
    for(int i = 0; i < length; i++)
        data[i] = (max - min) * ((double) rand() / (double) RAND_MAX) + min;
}

Matrix::Matrix(Matrix* other) : Matrix(other->nRows, other->nCols) { 
    for(int i = 0; i < other->length; i++)
        this->data[i] = other->data[i];

    this->nRows = other->nRows;
    this->nCols = other->nCols;
}

Matrix::~Matrix() { 
    free(data);
    free(data2D);
}

Matrix* Matrix::setData(double* data) {
    this->data = data;

    updateData2D();

    return this;
}

void Matrix::updateData2D() {
    for(int r = 0; r < nRows; r++)
        data2D[r] = &data[nCols * r];
}

// Matrix* Matrix::setData(double* data, int nRows, int nCols) {
//     this->data = data;
//     this->nRows = nRows;
//     this->nCols = nCols;
//     this->length = nRows * nCols;

//     return this;
// }

Matrix* Matrix::copyData(Matrix* other) {
    if(this->length != other->length)
        throw std::invalid_argument("Unequal Copy Result Size");

    for(int i = 0; i < other->length; i++)
        this->data[i] = other->data[i];

    return this;
}

double Matrix::at(int row, int col) {
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

Matrix* Matrix::set(int row, int col, double value) {
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

double Matrix::vectorMagnitude() {
    if(nRows != 1 && nCols != 1) {
        throw std::invalid_argument("Cannot compute magnitude of a matrix");
    }

    double sum = 0.0;

    for(int i = 0; i < length; i++) {
        sum += std::pow(data[i], 2.0);
    }

    return sqrt(sum);
}

Matrix* Matrix::add(Matrix* other, Matrix* result) {
    if(other->nCols != nCols || result->nCols != nCols || 
        other->nRows != nRows || result->nRows != nRows)
        throw std::invalid_argument("Unequal Matrix Size When Adding");

    for(int i = 0; i < length; i++)
        result->data[i] = data[i] + other->data[i];

    return result;
}

Matrix* Matrix::mAdd(Matrix* other) {
    if(other->nCols != nCols || other->nRows != nRows)
        throw std::invalid_argument("Unequal Matrix Size When Adding");

    for(int i = 0; i < length; i++)
        data[i] += other->data[i];

    return this;
}

Matrix* Matrix::add(double adder, Matrix* result) {
    if(result->nCols != this->nCols || result->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Adding");

    for(int i = 0; i < length; i++)
        result->data[i] = data[i] + adder;

    return result;
}

Matrix* Matrix::mAdd(double adder) {
    for(int i = 0; i < length; i++)
        data[i] += adder;

    return this;
}

Matrix* Matrix::subtract(Matrix* other, Matrix* result) {
    if(other->nCols != this->nCols || result->nCols != this->nCols || 
        other->nRows != this->nRows || result->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Subtracting");

    for(int i = 0; i < length; i++)
        result->data[i] = data[i] - other->data[i];

    return result;
}

Matrix* Matrix::mSubtract(Matrix* other) {
    if(other->nCols != this->nCols || other->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Subtracting");

    for(int i = 0; i < length; i++)
        data[i] -= other->data[i];

    return this;
}

Matrix* Matrix::pow(double p, Matrix* result) {
    if(result->nCols != this->nCols || result->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Powing");

    for(int i = 0; i < length; i++)
        result->data[i] = std::pow(data[i], p);

    return result;
}

Matrix* Matrix::elementProduct(Matrix* other, Matrix* result) {
    if(other->nCols != this->nCols || result->nCols != this->nCols || 
        other->nRows != this->nRows || result->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Element Producting");

    for(int i = 0; i < length; i++)
        result->data[i] = data[i] * other->data[i];

    return result;
}

Matrix* Matrix::mElementProduct(Matrix* other) {
    if(other->nCols != this->nCols || other->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Element Producting");

    for(int i = 0; i < length; i++)
        data[i] *= other->data[i];

    return this;
}

Matrix* Matrix::elementDivision(Matrix* other, Matrix* result) {
    if(other->nCols != this->nCols || result->nCols != this->nCols || 
        other->nRows != this->nRows || result->nRows != this->nRows)
        throw std::invalid_argument("Unequal Matrix Size When Dividing");

    for(int i = 0; i < length; i++)
        result->data[i] = data[i] / other->data[i];

    return result;
}

double Matrix::segmentedElementWiseProductSum(Matrix* other, int startRow, int startCol) {
    double sum = 0.0;

    for(int row = startRow; row < other->nRows + startRow; row++) {
    for(int col = startCol; col < other->nCols + startCol; col++) {
        sum += data[(row * this->nCols) + col] * other->data[((row - startRow) * other->nCols) + (col - startCol)];
    }}

    // int smallIndex = 0;
    // int largeIndex = (startRow * nCols) + startCol;
    // for(int row = 0; row < other->nRows; row++) {
    //     for(int i = 0; i < other->nCols; i++) {
    //         sum += data[largeIndex] * other->data[smallIndex];

    //         smallIndex++;
    //         largeIndex++;
    //     }

    //     largeIndex += nCols - other->nCols + 1;
    // }

    return sum;
}

double Matrix::segmentedFlippedElementWiseProductSum(Matrix* other, int startRow, int startCol) {
    double sum = 0.0;

    for(int row = startRow; row < other->nRows + startRow; row++) {
    for(int col = startCol; col < other->nCols + startCol; col++) {
        sum += data[(row * this->nCols) + col] * other->data[(((other->nRows - row - 1) + startRow) * other->nCols) + ((other->nCols - col - 1) + startCol)];
    }}

    return sum;
}

Matrix* Matrix::scale(double scalar, Matrix* result) {
    if(this->nRows != result->nRows || this->nCols != result->nCols)
        throw std::invalid_argument("Invalid Result Size When Scaling");

    for(int i = 0; i < length; i++) 
        result->data[i] = this->data[i] * scalar;
    return result;
}

Matrix* Matrix::mScale(double scalar) {
    for(int i = 0; i < length; i++) 
        data[i] *= scalar;
    return this;
}

Matrix* Matrix::multiply(Matrix* other, Matrix* result) {
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
            // sum += data[(i * nCols) + k] * other->data[(k * other->nCols) + j];
            sum += data2D[i][k] * other->data2D[k][j];
        }

        // result->data[(i * result->nCols) + j] = sum;
        result->data2D[i][j] = sum;
    }}

    return result;
}

Matrix* Matrix::multiplyOtherTransposed(Matrix* other, Matrix* result) {
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
            // sum += data[(i * nCols) + k] * other->data[(j * other->nCols) + k];
            sum += data2D[i][k] * other->data2D[j][k];
        }

        // result->data[(i * result->nCols) + j] = sum;
        result->data2D[i][j] = sum;
    }}

    return result;
}

Matrix* Matrix::multiplyOtherTransposedAdded(Matrix* other, Matrix* adder, Matrix* result) {
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
            // sum += data[(i * nCols) + k] * other->data[(j * other->nCols) + k];
            sum += data2D[i][k] * other->data2D[j][k];
        }

        // result->data[(i * result->nCols) + j] = sum + adder->data[(i * result->nCols) + j];
        result->data2D[i][j] = sum + adder->data2D[i][j];
    }}

    return result;
}

Matrix* Matrix::multiplyInputTransposed(Matrix* other, Matrix* result) {
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
            // sum += data[(k * nCols) + i] * other->data[(k * other->nCols) + j];
            sum += data2D[k][i] * other->data2D[k][j];
        }

        // result->data[(i * result->nCols) + j] = sum;
        result->data2D[i][j] = sum;
    }}

    return result;
}

Matrix* Matrix::multiplyInputTransposedAdded(Matrix* other, Matrix* adder, Matrix* result) {
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
            // sum += data[(k * nCols) + i] * other->data[(k * other->nCols) + j];
            sum += data2D[k][i] * other->data2D[k][j];
        }

        // result->data[(i * result->nCols) + j] = sum + adder->data[(i * result->nCols) + j];
        result->data2D[i][j] = sum + adder->data2D[i][j];
    }}

    return result;
}

Matrix* Matrix::transpose(Matrix* result) {
    if(result->nRows != this->nCols || result->nCols != this->nRows)
        throw std::invalid_argument("Invalid Result Matrix Size to Transpose");
    
    for(int row = 0; row < this->nRows; row++) {
    for(int col = 0; col < this->nCols; col++) {
        // result->set(col, row, this->at(row, col));
        result->data2D[col][row] = data2D[row][col];
    }}

    return result;
}

Matrix* Matrix::dialatePad(int dialate, int pad, Matrix* result) {
    if(result->getNRows() != ((nRows - 1) * (dialate - 1)) + nRows + (2 * pad) || result->getNCols() != ((nRows - 1) * (dialate - 1)) + nCols + (2 * pad))
        throw std::invalid_argument("Invalid result size to dialate");

    int index = 0;
    for(int row = pad; row < result->getNRows() - pad; row += dialate) {
    for(int col = pad; col < result->getNCols() - pad; col += dialate) {
        result->set(row, col, data[index]);

        index++;
    }}

    return result;
}

Matrix* Matrix::forEach(double (*function)(double), Matrix* result) {
    if(this->nRows != result->nRows || this->nCols != result->nCols)
        throw std::invalid_argument("Invalid number of rows and cols for a for each");

    for(int i = 0; i < length; i++)
        result->data[i] = function(this->data[i]);

    return result;
}

Matrix* Matrix::mForEach(double (*function)(double)) {
    return forEach(function, this);
}

/*
*   This will set all of the data values to 0
*/
Matrix* Matrix::clear() { 
    for(int i = 0; i < length; i++)
        this->data[i] = 0.0;
    return this;
}

/*
*   Sums together the values of this matrix
*/
double Matrix::sumElements() { 
    double result = 0.0;
    for(int i = 0; i < length; i++)
        result += this->data[i];

    return result;
}

double Matrix::greatest() {
    double g = data[0];

    for(int i = 1; i < length; i++)
        if(data[i] > g)
            g = data[i];

    return g;
}

int Matrix::greatestIndex() {
    int i = 0;
    for(int j = 1; j < length; j++) 
        if(data[j] > data[i])
            i = j;
    
    return i;
}

double* Matrix::getData() { return data; }
int Matrix::getNRows() { return nRows; }
int Matrix::getNCols() { return nCols; }
int Matrix::getLength() { return length; }

void Matrix::print() {
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