#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#include "./Matrix/Matrix.cpp"

using namespace std;

class ConvolutionalLayer {
    public:
        Matrix** kernals;
        double* bias;

        int padding, stride;
        int kernalCount, kernalSize;
        int inputDimensions;

        int inputRows, inputCols;

        Matrix* input;
        Matrix* output;

        Matrix* toRetError;
        Matrix* errorConvolutionResult;

        Matrix* gradients;
        Matrix* gradientConvolutionOutput;

        Matrix** kernalVelocities;
        double* biasVelocities;

        int outputRows, outputCols;

        double learningRate = 0.01;
        double velocityCoefficient = 0.9;

        /*
        *   General convolutional method -> Different parts of propogation need to use convolution
        */
        Matrix* convolute(Matrix* input, Matrix** kernals, Matrix* output, double* bias, int kernalCount, int inputDimensions, int stride, int padding) {
            if((&input[0])->getPadding() != padding) {
                for(int i = 0; i < kernalCount; i++) {
                    (&input[i])->setPadding(padding);
                }
            }

            for(int row = 0; row <= (&input[0])->getNRows() - kernalSize; row += stride) {
            for(int col = 0; col <= (&input[0])->getNCols() - kernalSize; col += stride) {
                for(int set = 0; set < kernalCount; set++) {
                    double sum = 0.0;
                    for(int kernal = 0; kernal < inputDimensions; kernal++)
                        sum += (&input[kernal])->elementWiseProduct(&kernals[set][kernal], row, col, kernalSize, kernalSize);

                    if(bias != NULL)
                        sum += bias[set];

                    (&output[set])->set(row / stride, col / stride, sum);
                }
            }}

            return output;
        }

        Matrix* convolute(Matrix* input, Matrix* kernals, Matrix* output, int kernalSize, int kernalCount, double* bias, int stride, int padding) {
            if((&input[0])->getPadding() != padding) {
                for(int i = 0; i < kernalCount; i++) {
                    (&input[i])->setPadding(padding);
                }
            }

            for(int row = 0; row <= (&input[0])->getNRows() - kernalSize; row += stride) {
            for(int col = 0; col <= (&input[0])->getNCols() - kernalSize; col += stride) {
                for(int set = 0; set < kernalCount; set++) {
                    double sum = 0.0;
                    sum = (&input[set])->elementWiseProduct(&kernals[set], row, col, kernalSize, kernalSize);

                    if(bias != NULL)
                        sum += bias[set];

                    (&output[set])->set(row / stride, col / stride, sum);
                }
            }}

            return output;
        }

    public:
        ConvolutionalLayer(int kernalCount, int kernalSize, int inputRows, int inputCols, int stride, int inputDimensions) {
            this->kernalCount = kernalCount;
            this->kernalSize = kernalSize;
            this->inputRows = inputRows;
            this->inputCols = inputCols;
            this->stride = stride;
            this->inputDimensions = inputDimensions;
            
            padding = 0;

            //Kernal Init
            kernals = new Matrix*[kernalCount];
            kernalVelocities = new Matrix*[kernalCount];
            for(int i = 0; i < kernalCount; i++) {
                kernals[i] = (Matrix*) malloc(sizeof(Matrix) * inputDimensions);
                kernalVelocities[i] = (Matrix*) malloc(sizeof(Matrix) * inputDimensions);

                for(int j = 0; j < inputDimensions; j++) {
                    new (&(kernals[i][j])) Matrix(kernalSize, kernalSize, -1.0, 1.0);
                    new (&(kernalVelocities[i][j])) Matrix(kernalSize, kernalSize);
                }
            }

            //Input
            // input = (Matrix*) malloc(sizeof(Matrix));
            // new (&input[0]) Matrix(inputRows, inputCols);

            //Output
            double outputRows = ((inputRows - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
            double outputCols = ((inputCols - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw invalid_argument("Invalid convolution parameters");
            
            this->outputRows = (int) outputRows;
            this->outputCols = (int) outputCols;

            output = (Matrix*) malloc(sizeof(Matrix) * kernalCount);

            for(int i = 0; i < kernalCount; i++)
                new (&output[i]) Matrix(this->outputRows, this->outputCols);

            //Biases
            bias = (double*) calloc(kernalCount, sizeof(double));
            biasVelocities = (double*) calloc(kernalCount, sizeof(double));

            for(int i = 0; i < kernalCount; i++) {
                bias[i] = 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;
            }

            //Place-Holder Terms

            //Error Convolution
            toRetError = (Matrix*) malloc(sizeof(Matrix) * inputDimensions);
            errorConvolutionResult = (Matrix*) malloc(sizeof(Matrix) * kernalCount);

            //                  Change this when more information is given about the next layer!!!
            //The number is the size of the incoming error
            double errorConvOutputRows = ((2.0 - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
            double errorConvOutputCols = ((2.0 - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;

            if((int) errorConvOutputRows != errorConvOutputRows || (int) errorConvOutputCols != errorConvOutputCols)
                throw invalid_argument("Invalid error convolution parameters");
            
            for(int i = 0; i < kernalCount; i++)
                new (&(errorConvolutionResult[i])) Matrix((int) errorConvOutputRows, (int) errorConvOutputCols);

            for(int i = 0; i < inputDimensions; i++)
                new (&(toRetError[i])) Matrix((int) errorConvOutputRows, (int) errorConvOutputCols);

            //Gradient
            gradients = (Matrix*) malloc(sizeof(Matrix) * kernalCount);
            gradientConvolutionOutput = (Matrix*) malloc(sizeof(Matrix) * inputDimensions);

            //change this when there is more information about the next layer
            //The number is meant to be the size of the incoming error
            double gradientConvOutputRows = ((inputRows - 2) / (double) stride) + 1.0;
            double gradientConvOutputCols = ((inputCols - 2) / (double) stride) + 1.0;

            if((int) gradientConvOutputRows != gradientConvOutputRows || (int) gradientConvOutputCols != gradientConvOutputCols)
                throw invalid_argument("Invalid error convolution parameters");
            
            for(int i = 0; i < kernalCount; i++)
                new (&(gradients[i])) Matrix((int) gradientConvOutputRows, (int) gradientConvOutputCols);

            for(int i = 0; i < inputDimensions; i++) 
                new (&(gradientConvolutionOutput[i])) Matrix((int) gradientConvOutputRows, (int) gradientConvOutputCols);
        }

        Matrix* feedForward(Matrix* input) { 
            this->input = input;

            convolute(input, kernals, output, bias, kernalCount, inputDimensions, stride, padding);

            return output;
        }

        Matrix backpropogate(Matrix* error) {
            //Next Error
            // for(int j = 0; j < inputDimensions; j++) {
            //     (&toRetError[j])->clear();

            //     for(int i = 0; i < kernalCount; i++) {
            //         convolute(&error[i], (&(&kernals[i])[j]), &errorConvolutionResult[i], NULL, 1, 1, stride, padding);
            //         (&toRetError[j])->mAdd(&errorConvolutionResult[i]);
            //     }
            // }

            //Gradient
            for(int i = 0; i < kernalCount; i++) {
                (&gradients[i])->clear();
                for(int j = 0; j < inputDimensions; j++) {
                    convolute(&input[j], &error[i], &gradientConvolutionOutput[j], (&error[i])->getNRows(), 1, NULL, stride, padding);

                    (&gradients[i])->mAdd(&gradientConvolutionOutput[j]);
                }
                (&gradients[i])->mScale(learningRate);

                //Momentum
                biasVelocities[i] *= velocityCoefficient;
                biasVelocities[i] -= (&gradients[i])->sumElements();
                bias[i] += biasVelocities[i];

                for(int kernal = 0; kernal < inputDimensions; kernal++) {
                    (&kernalVelocities[i][kernal])->mScale(velocityCoefficient);
                    (&kernalVelocities[i][kernal])->mSubtract(&gradients[i]);
                    (&kernals[i][kernal])->mAdd(&kernalVelocities[i][kernal]);
                }
            }

            return toRetError;
        }
};

int main() { 
    srand(48563);

    ConvolutionalLayer conv(2, 2, 4, 4, 2, 1);
    (&conv.kernals[0][0])->set(0, 0, .2);
    (&conv.kernals[0][0])->set(0, 1, .2);
    (&conv.kernals[0][0])->set(1, 0, .2);
    (&conv.kernals[0][0])->set(1, 1, .2);

    (&conv.kernals[1][0])->set(0, 0, .2);
    (&conv.kernals[1][0])->set(0, 1, .2);
    (&conv.kernals[1][0])->set(1, 0, .2);
    (&conv.kernals[1][0])->set(1, 1, .2);

    Matrix m(4, 4);

    int n = 0;
    for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
        m.set(row, col, n / 20.0);

        n++;
    }}

    cout << "Kernals" << endl;
    conv.kernals[0][0].print();

    cout << endl << "IN" << endl;
    m.print();

    Matrix* out = conv.feedForward(&m);

    cout << endl << "Initial" << endl;
    out->print();

    Matrix* error = (Matrix*) malloc(sizeof(Matrix) * 2);
    new (&error[0]) Matrix(2, 2);
    new (&error[1]) Matrix(2, 2);

    Matrix answer1(2, 2);
    answer1.set(0, 0, 0.1);
    answer1.set(0, 1, 0.3);
    answer1.set(1, 0, 0.6);
    answer1.set(1, 1, 0.9);

    Matrix answer2(2, 2);
    answer2.set(0, 0, 0.1);
    answer2.set(0, 1, 0.0);
    answer2.set(1, 0, 0.6);
    answer2.set(1, 1, 0.9);

    for(int i = 0; i < 5000; i++) {
        out[0].subtract(&answer1, &(error[0]));
        out[1].subtract(&answer2, &(error[1]));

        // cout << endl;
        // cout << "Error" << endl;

        // error.print();

        conv.backpropogate(error);

        // cout << endl;

        // cout << "IN" << endl;
        // m.print();

        out = conv.feedForward(&m);

        cout << endl;
        cout << "OUT" << endl;

        out[0].print();

        cout << endl;

        out[1].print();
    }

    // cout << endl;

    // error[0].print();

    // cout << endl;

    // error[1].print();
}