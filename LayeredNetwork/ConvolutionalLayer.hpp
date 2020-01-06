#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "MomentumUpdater.hpp"

#endif

class ConvolutionalLayer : public Layer {
    private:
        Matrix** weights;
        Matrix** weightGradient;
        Updater** weightUpdater;

        Matrix* bias;
        Matrix* biasGradient;
        Updater* biasUpdater;

        Matrix* errorDialatedPadded;

        int kernalSize;
        int stride;

        Matrix* convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped) {
            return convolute(input, filter, 0, output, s, flipped, NULL);
        }

        Matrix* convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped, Matrix* adder) {
            return convolute(input, filter, 0, output, s, flipped, adder);
        }

        Matrix* convolute(Matrix* input, Matrix* filter, double bias, Matrix* output, int s, bool flipped, Matrix* adder) {
            double outputRows = ((input->getNRows() - filter->getNRows()) / ((double) s)) + 1.0;
            double outputCols = ((input->getNCols() - filter->getNRows()) / ((double) s)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw std::invalid_argument("Invalid convolution parameters");

            if((int) outputRows != output->getNRows() || (int) outputCols != output->getNCols())
                throw std::invalid_argument("convolution parameters don't match paramater constraints");

            for(int row = 0; row <= input->getNRows() - filter->getNRows(); row += s) {
            for(int col = 0; col <= input->getNCols() - filter->getNRows(); col += s) {
                double sum = 0.0;
                if(flipped)
                    sum = input->flippedElementWiseProduct(filter, row, col, filter->getNRows(), filter->getNRows());
                else
                    sum = input->elementWiseProduct(filter, row, col, filter->getNRows(), filter->getNRows());

                sum += bias;

                if(adder == NULL)
                    output->set(row / s, col / s, sum);
                else
                    output->set(row / s, col / s, sum + adder->at(row / s, col / s));
            }}

            return output;
        }

    public:
        ConvolutionalLayer(NetworkInformation* networkInformation, Layer** networkLayers, int index, int kernalCount, int kernalSize, int stride) : Layer(networkInformation, networkLayers, index) {
            this->kernalSize = kernalSize;
            this->stride = stride;
            outputMatrixCount = kernalCount;
        }

        void initialize() {
            parameterGradientInfoLength = parameterLength = (kernalSize * kernalSize * outputMatrixCount * inputMatrixCount) + outputMatrixCount;

            parameters = (double*) calloc(parameterLength, sizeof(double));
            parameterGradientInfo = (double*) calloc(parameterGradientInfoLength, sizeof(double));

            weights = new Matrix*[outputMatrixCount];
            weightGradient = new Matrix*[outputMatrixCount];
            weightUpdater = new Updater*[outputMatrixCount];
            for(int i = 0; i < outputMatrixCount; i++) {
                weights[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
                weightGradient[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
                weightUpdater[i] = (MomentumUpdater*) malloc(sizeof(MomentumUpdater) * inputMatrixCount);

                for(int j = 0; j < inputMatrixCount; j++) {
                    new (&(weights[i][j])) Matrix(&(parameters[((i * inputMatrixCount) + j) * kernalSize * kernalSize]), kernalSize, kernalSize, -1.0, 1.0);
                    new (&(weightGradient[i][j])) Matrix(&(parameterGradientInfo[((i * inputMatrixCount) + j) * kernalSize * kernalSize]), kernalSize, kernalSize);
                    new (&(weightUpdater[i][j])) MomentumUpdater(networkInformation, kernalSize, kernalSize);
                }
            }

            bias = new Matrix(&(parameters[parameterLength - outputMatrixCount]), 1, outputMatrixCount, -1.0, 1.0);
            biasGradient = new Matrix(&(parameterGradientInfo[parameterLength - outputMatrixCount]), 1, outputMatrixCount);
            biasUpdater = new MomentumUpdater(networkInformation, 1, outputMatrixCount);

            double outputRows = ((inputNRows - kernalSize) / ((double) stride)) + 1.0;
            double outputCols = ((inputNCols - kernalSize) / ((double) stride)) + 1.0;

            if((int) outputRows != outputRows || (int) outputCols != outputCols)
                throw std::invalid_argument("Invalid constructor convolution paramaters");

            outputNRows = (int) outputRows;
            outputNCols = (int) outputCols;

            outputInfoLength = outputNRows * outputNCols * outputMatrixCount;
            outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

            output = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
            for(int i = 0; i < outputMatrixCount; i++)
                new (&output[i]) Matrix(&outputInfo[outputNRows * outputNCols * i], outputNRows, outputNCols);

            errorDialatedPadded = new Matrix(((outputNRows - 1) * (stride - 1)) + outputNRows + (2 * (kernalSize - 1)), 
                                             ((outputNCols - 1) * (stride - 1)) + outputNCols + (2 * (kernalSize - 1)));

            if(index != 0) {
                input = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

                for(int i = 0; i < inputMatrixCount; i++) {
                    new (&input[i]) Matrix(&(layers[index - 1]->getOutputInfo()[inputNCols * inputNRows * i]), inputNRows, inputNCols);
                }

                layerGradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
                layerGradientInfoLength = layers[index - 1]->getOutputInfoLength();
                layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

                for(int i = 0; i < inputMatrixCount; i++) {
                    new (&layerGradient[i]) Matrix(&layerGradientInfo[inputNRows * inputNCols * i], inputNRows, inputNCols);
                }
            }
        }

        void postInitialize() {
            if(index != networkInformation->getAmountOfLayers() - 1) {
                error = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
                for(int i = 0; i < outputMatrixCount; i++)
                    new (&error[i]) Matrix(&(layers[index + 1]->getLayerGradientInfo()[outputNRows * outputNCols * i]), outputNRows, outputNCols);
            }
        }

        Matrix* feedForward() {
            for(int i = 0; i < outputMatrixCount; i++) {
                for(int inputMatrixIndex = 0; inputMatrixIndex < inputMatrixCount; inputMatrixIndex++) {
                    if(inputMatrixIndex == 0)
                        convolute(&input[inputMatrixIndex], &weights[i][inputMatrixIndex], bias->at(0, i), &output[i], stride, false, NULL);
                    else
                        convolute(&input[inputMatrixIndex], &weights[i][inputMatrixIndex], bias->at(0, i), &output[i], stride, false, &output[i]);
                }
            }

            return output;
        }

        Matrix* calculateGradient() {
            if(index != 0) {
                //Layer Gradient Calculation
                for(int i = 0; i < inputMatrixCount; i++) {
                    for(int j = 0; j < outputMatrixCount; j++) {
                        (&error[j])->dialatePad(errorDialatedPadded, stride, kernalSize - 1);

                        if(i == 0 && j == 0)
                            convolute(errorDialatedPadded, &weights[j][i], (&layerGradient[i]), 1, true);
                        else
                            convolute(errorDialatedPadded, &weights[j][i], (&layerGradient[i]), 1, true, (&layerGradient[i]));
                    }
                }
            }

            //Weight Gradient Calculation
            for(int i = 0; i < outputMatrixCount; i++) {
                for(int j = 0; j < inputMatrixCount; j++) {
                    if(networkInformation->getBatchIndex() == 0)
                        convolute(&input[j], &error[i], &(weightGradient[i][j]), (int) (input->getNRows() - error->getNRows()) / (kernalSize - 1), false);
                    else
                        convolute(&input[j], &error[i], &(weightGradient[i][j]), (int) (input->getNRows() - error->getNRows()) / (kernalSize - 1), false, &(weightGradient[i][j]));
                }

                if(networkInformation->getBatchIndex() == 0)
                    biasGradient->getData()[i] = (&error[i])->sumElements();
                else
                    biasGradient->getData()[i] += (&error[i])->sumElements();
            }

            return layerGradient;
        }

        void update() {
            for(int i = 0; i < outputMatrixCount; i++)
            for(int j = 0; j < inputMatrixCount; j++)
                (&weightUpdater[i][j])->update(&weights[i][j], &weightGradient[i][j]);

            biasUpdater->update(bias, biasGradient);
        }
};