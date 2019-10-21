#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#include "./Matrix/Matrix.cpp"

using namespace std;

class ConvolutionalLayer {
    public:
        Matrix** kernals;
        double* biases;

        int padding, stride;
        int kernalCount, kernalSize;
        int inputDimensions;

        int inputRows, inputCols;

        Matrix* input;
        Matrix* output;

        int outputRows, outputCols;

        Matrix* convolute(Matrix* input, Matrix** kernals, Matrix* output, int stride, int padding) { 

        }

    public:
        ConvolutionalLayer(int kernalCount) {
            padding = 0;
            this->kernalCount = 1;
            kernalSize =  2;
            inputDimensions = 1;

            inputRows = 4;
            inputCols = 4;
            stride = 2; 

            kernals = new Matrix*[kernalCount];
            for(int i = 0; i < kernalCount; i++) {
                kernals[i] = (Matrix*) malloc(sizeof(Matrix) * inputDimensions);

                for(int j = 0; j < inputDimensions; j++)
                    new (&(kernals[i][j])) Matrix(kernalSize, kernalSize, -1.0, 1.0);
            }

            input = (Matrix*) malloc(sizeof(Matrix));
            new (&input[0]) Matrix(inputRows, inputCols);

            double outputRows = ((inputRows - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
            double outputCols = ((inputCols - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw invalid_argument("Invalid convolution parameters");
            
            this->outputRows = (int) outputRows;
            this->outputCols = (int) outputCols;

            // cout << this->outputRows << endl;

            output = (Matrix*) malloc(sizeof(Matrix));
            new (&output[0]) Matrix(this->outputRows, this->outputCols);
        }

        Matrix* feedForward(Matrix* input) { 
            for(int row = 0; row < inputRows; row += stride) {
            for(int col = 0; col < inputCols; col += stride) {
                for(int set = 0; set < kernalCount; set++) {
                    double sum = 0.0;
                    for(int kernal = 0; kernal < inputDimensions; kernal++) {
                        sum += (&input[kernal])->elementWiseProduct(&kernals[set][kernal], row, col, kernalSize, kernalSize);
                    }

                    (&output[set])->set(row / stride, col / stride, sum);
                }
            }}

            return output;
        }
};

int main() { 
    ConvolutionalLayer conv(1);

    int n = 0;
    for(int row = 0; row < 2; row++) {
    for(int col = 0; col < 2; col++) {
        conv.kernals[0][0].set(row, col, n);

        n++;
    }}

    Matrix m(4, 4);

    n = 0;
    for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
        m.set(row, col, n);

        n++;
    }}

    cout << "Kernals" << endl;
    conv.kernals[0][0].print();

    cout << "IN" << endl;
    m.print();

    Matrix* out = conv.feedForward(&m);

    out->print();
}