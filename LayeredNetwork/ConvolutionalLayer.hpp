#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "MomentumUpdater.hpp"

#endif

#ifndef RELU_FUNCTION_HPP
#define RELU_FUNCTION_HPP

#include "ReluFunction.hpp"

#endif

#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "SigmoidFunction.hpp"

#endif

class ConvolutionalLayer : public Layer {
    private:
        Matrix** weights;
        Matrix* biases;

        Matrix* nets;
        Matrix* convolutionOutput;

        Matrix* weightGradient;
        Matrix* biasGradient;

        Matrix* input;

        Matrix* errorDialatedPadded;
        Matrix* layerGradientConvolutionOutput;

        MomentumUpdater** weightUpdater;
        Updater* biasUpdater;

        ActivationFunction* activationFunction;

        double learningRate = 0.01;

        int kernalSize;
        int stride;

        Matrix* bias;

        Matrix* convolute(Matrix* input, Matrix* filter, Matrix* output, int s) {
            return convolute(input, filter, 0, output, s);
        }

        Matrix* convolute(Matrix* input, Matrix* filter, double bias, Matrix* output, int s) {
            double outputRows = ((input->getNRows() - filter->getNRows()) / ((double) s)) + 1.0;
            double outputCols = ((input->getNCols() - filter->getNRows()) / ((double) s)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw std::invalid_argument("Invalid convolution parameters");

            if((int) outputRows != output->getNRows() || (int) outputCols != output->getNCols())
                throw std::invalid_argument("convolution parameters don't match paramater constraints");

            for(int row = 0; row <= input->getNRows() - filter->getNRows(); row += s) {
            for(int col = 0; col <= input->getNCols() - filter->getNRows(); col += s) {
                double sum = input->elementWiseProduct(filter, row, col, filter->getNRows(), filter->getNRows());

                sum += bias;

                output->set(row / s, col / s, sum);
            }}

            return output;
        }

    public:
        ConvolutionalLayer(Layer** networkLayers, int index, int kernalCount, int kernalSize, int stride) : Layer(networkLayers, index) {
            this->kernalSize = kernalSize;
            this->stride = stride;
            outputMatrixCount = kernalCount;
        }

        void initialize() {
            // weights = new Matrix(kernalSize, kernalSize, -1.0, 1.0);
            // weights = new Matrix*[outputMatrixCount];
            // weightGradient = new Matrix(kernalSize, kernalSize);

            weights = new Matrix*[outputMatrixCount];
            weightUpdater = new MomentumUpdater*[outputMatrixCount];
            for(int i = 0; i < outputMatrixCount; i++) {
                weights[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
                weightUpdater[i] = (MomentumUpdater*) malloc(sizeof(MomentumUpdater) * inputMatrixCount);

                for(int j = 0; j < inputMatrixCount; j++) {
                    new (&(weights[i][j])) Matrix(kernalSize, kernalSize, -1.0, 1.0);
                    new (&(weightUpdater[i][j])) MomentumUpdater(kernalSize, kernalSize);
                }
            }

            bias = new Matrix(1, outputMatrixCount, -1.0, 1.0);
            biasGradient = new Matrix(1, outputMatrixCount);

            weightGradient = new Matrix(kernalSize, kernalSize);

            // weightUpdater = new MomentumUpdater(outputMatrixCount, kernalSize, kernalSize);
            biasUpdater = new MomentumUpdater(1, outputMatrixCount);

            activationFunction = new ReluFunction();

            // biases = new Matrix(outputNRows, 1, -1.0, 1.0);
            // biasGradient = new Matrix(outputNRows, 1, -1.0, 1.0);

            double outputRows = ((inputNRows - kernalSize) / ((double) stride)) + 1.0;
            double outputCols = ((inputNCols - kernalSize) / ((double) stride)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw std::invalid_argument("Invalid constructor convolution paramaters");

            outputNRows = (int) outputRows;
            outputNCols = (int) outputCols;

            nets = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
            output = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);

            for(int i = 0; i < outputMatrixCount; i++) {
                new (&nets[i]) Matrix(outputNRows, outputNCols);
                new (&output[i]) Matrix(outputNRows, outputNCols);
            }

            convolutionOutput = new Matrix(outputNRows, outputNCols);

            errorDialatedPadded = new Matrix(((outputNRows - 1) * (stride - 1)) + outputNRows + (2 * (kernalSize - 1)), 
                                             ((outputNCols - 1) * (stride - 1)) + outputNCols + (2 * (kernalSize - 1)));

            layerGradientConvolutionOutput = new Matrix(inputNRows, inputNCols);

            gradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            for(int i = 0; i < inputMatrixCount; i++) {
                new (&gradient[i]) Matrix(inputNRows, inputNCols);
            }
        }

        Matrix* feedForward(Matrix* input) {
            this->input = input;

            // weights->multiply(input, nets);
            for(int i = 0; i < outputMatrixCount; i++) {
                (&nets[i])->clear();
                for(int inputMatrixIndex = 0; inputMatrixIndex < inputMatrixCount; inputMatrixIndex++) {
                    convolute(&input[inputMatrixIndex], &weights[i][inputMatrixIndex], bias->at(0, i), convolutionOutput, stride);
                    (&nets[i])->mAdd(convolutionOutput);
                }

                activationFunction->applyFunction(&nets[i], &output[i]);
            }

            return output;
        }

        Matrix* backpropogate(Matrix* error) {
            activationFunction->applyDerivativeFunction(nets);
            error->elementProduct(nets, error);

            //Weight Gradient Calculation
            // int s = ;
            for(int i = 0; i < outputMatrixCount; i++) {
                for(int j = 0; j < inputMatrixCount; j++) {
                    convolute(&input[j], &error[i], weightGradient, (int) (input->getNRows() - error->getNRows()) / (kernalSize - 1));

                    //Parameter Update
                    weightGradient->mScale(learningRate);
                    (&(weightUpdater[i][j]))->update(&(weights[i][j]), weightGradient);
                }

                biasGradient->getData()[i] = error->sumElements() * learningRate;
            }

            biasUpdater->update(bias, biasGradient);

            //Layer Gradient Calculation
            for(int i = 0; i < inputMatrixCount; i++) {
                (&gradient[i])->clear();
                for(int j = 0; j < outputMatrixCount; j++) {
                    (&error[j])->dialatePad(errorDialatedPadded, stride, kernalSize - 1);

                    (&weights[j][i])->mFlip();
                    convolute(errorDialatedPadded, &weights[j][i], layerGradientConvolutionOutput, 1);
                    (&weights[j][i])->mFlip();

                    (&gradient[i])->mAdd(layerGradientConvolutionOutput);
                }
            }

            return gradient;
        }
};