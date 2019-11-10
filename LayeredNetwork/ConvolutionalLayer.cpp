#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

class ConvolutionalLayer: public Layer {
    public:
        Matrix** kernals;
        double* bias;

        int padding, stride;
        int kernalCount, kernalSize;

        Matrix* input;

        Matrix* toRetError;
        Matrix* errorConvolutionResult;

        Matrix* gradients;
        Matrix* gradientConvolutionOutput;

        Matrix** kernalVelocities;
        double* biasVelocities;

        double learningRate = 0.01;
        double velocityCoefficient = 0.9;

        /*
        *   General convolutional method -> Different parts of propogation need to use convolution
        */
        Matrix* convolute(Matrix* input, Matrix** kernals, Matrix* output, double* bias, int kernalCount, int inputDimensions, int stride, int padding) {
            // if((&input[0])->getPadding() != padding) {
            //     for(int i = 0; i < kernalCount; i++) {
            //         (&input[i])->setPadding(padding);
            //     }
            // }

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
        ConvolutionalLayer(Layer** networkLayers, int index, int kernalCount, int kernalSize, int stride) : Layer(networkLayers, index) {
            this->kernalCount = kernalCount;
            this->kernalSize = kernalSize;
            this->stride = stride;
            
            padding = 0;
        }

        void initialize() {
            //Kernal Init
            kernals = new Matrix*[kernalCount];
            kernalVelocities = new Matrix*[kernalCount];
            for(int i = 0; i < kernalCount; i++) {
                kernals[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
                kernalVelocities[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

                for(int j = 0; j < inputMatrixCount; j++) {
                    new (&(kernals[i][j])) Matrix(kernalSize, kernalSize, -1.0, 1.0);
                    new (&(kernalVelocities[i][j])) Matrix(kernalSize, kernalSize);
                }
            }

            //Input
            // input = (Matrix*) malloc(sizeof(Matrix));
            // new (&input[0]) Matrix(inputRows, inputCols);

            //Output
            double outputRows = ((inputNRows - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
            double outputCols = ((inputNCols - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;

            printf("%f\n", outputRows);

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw std::invalid_argument("Invalid convolution parameters");
            
            this->outputNRows = (int) outputRows;
            this->outputNCols = (int) outputCols;

            output = (Matrix*) malloc(sizeof(Matrix) * kernalCount);

            for(int i = 0; i < kernalCount; i++)
                new (&output[i]) Matrix(this->outputNRows, this->outputNCols);

            //Biases
            bias = (double*) calloc(kernalCount, sizeof(double));
            biasVelocities = (double*) calloc(kernalCount, sizeof(double));

            for(int i = 0; i < kernalCount; i++) {
                bias[i] = 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;
            }

            /******* Place-Holder Terms *******/
            //Error Convolution
            toRetError = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            errorConvolutionResult = (Matrix*) malloc(sizeof(Matrix) * kernalCount);

            for(int i = 0; i < kernalCount; i++) 
                new (&(errorConvolutionResult[i])) Matrix(inputNRows, outputNCols);

            for(int i = 0; i < inputMatrixCount; i++)
                new (&(toRetError[i])) Matrix(inputNRows, inputNCols);

            //Gradient
            gradients = (Matrix*) malloc(sizeof(Matrix) * kernalCount);
            gradientConvolutionOutput = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

            //change this when there is more information about the next layer
            //The number is meant to be the size of the incoming error
            double gradientConvOutputRows = ((inputNRows - outputNRows) / (double) stride) + 1.0;
            double gradientConvOutputCols = ((inputNCols - outputNCols) / (double) stride) + 1.0;

            if((int) gradientConvOutputRows != gradientConvOutputRows || (int) gradientConvOutputCols != gradientConvOutputCols)
                throw std::invalid_argument("Invalid error convolution parameters");
            
            for(int i = 0; i < kernalCount; i++)
                new (&(gradients[i])) Matrix(kernalSize, kernalSize);

            for(int i = 0; i < inputMatrixCount; i++) 
                new (&(gradientConvolutionOutput[i])) Matrix(kernalSize, kernalSize);

            outputMatrixCount = kernalCount;
        }

        Matrix* feedForward(Matrix* input) { 
            this->input = input;

            convolute(input, kernals, output, bias, kernalCount, inputMatrixCount, stride, padding);

            return output;
        }

        Matrix* backpropogate(Matrix* error) {
            //Next Error This is not correct, will come back to this when the rest is done
            // for(int j = 0; j < inputDimensions; j++) { Dialation!
            //     (&toRetError[j])->clear();

            //     for(int i = 0; i < kernalCount; i++) {
            //         convolute(&error[i], (&(&kernals[i])[j]), &errorConvolutionResult[i], NULL, 1, 1, 1, inputRows - 1);
            //         (&toRetError[j])->mAdd(&errorConvolutionResult[i]);
            //     }
            // }

            // printf("Error rows: %d, cols: %d\n", error->getNRows(), error->getNCols());

            //Gradient
            for(int i = 0; i < kernalCount; i++) {
                (&gradients[i])->clear();
                for(int j = 0; j < inputMatrixCount; j++) {
                    convolute(&input[j], &error[i], &gradientConvolutionOutput[j], (&error[i])->getNRows(), 1, NULL, error->getNRows(), padding);

                    (&gradients[i])->mAdd(&gradientConvolutionOutput[j]);
                }
                (&gradients[i])->mScale(learningRate);

                //Momentum
                biasVelocities[i] *= velocityCoefficient;
                biasVelocities[i] -= (&gradients[i])->sumElements();
                bias[i] += biasVelocities[i];

                for(int kernal = 0; kernal < inputMatrixCount; kernal++) {
                    (&kernalVelocities[i][kernal])->mScale(velocityCoefficient);
                    (&kernalVelocities[i][kernal])->mSubtract(&gradients[i]);
                    (&kernals[i][kernal])->mAdd(&kernalVelocities[i][kernal]);
                }
            }

            return toRetError;
        }
};