#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "MomentumUpdater.hpp"

#endif

#ifndef RMS_UPDATER_HPP
#define RMS_UPDATER_HPP

#include "RMSUpdater.hpp"

#endif

#ifndef ADAM_UPDATER_HPP
#define ADAM_UPDATER_HPP

#include "AdamUpdater.hpp"

#endif

// #ifndef SIGMOID_FUNCTION_HPP
// #define SIGMOID_FUNCTION_HPP

// #include "SigmoidFunction.hpp"

// #endif

class FullyConnectedLayer : public Layer {
    private:
        Matrix* weights;
        Matrix* biases;

        Matrix* weightGradient;
        Matrix* biasGradient;

        Updater* weightUpdater;
        Updater* biasUpdater;

    public:
        FullyConnectedLayer(NetworkInformation* networkInformation, Layer** layers, int index, int numNodes) : Layer(networkInformation, layers, index) {
            outputMatrixCount = 1;
            outputNCols = 1;
            outputNRows = numNodes;
        }

        ~FullyConnectedLayer() {
            //Parameter and Gradients pointers are cleared by the Layer class. Just de-refrence the
            weights->setData(NULL);
            biases->setData(NULL);

            delete weights;
            delete biases;

            weightGradient->setData(NULL);
            biasGradient->setData(NULL);

            delete weightGradient;
            delete biasGradient;

            delete weightUpdater;
            delete biasUpdater;
        }

        void initialize() {
            int lenWeights = (outputNRows * inputNRows * inputNCols);
            parameterLength = lenWeights + outputNRows;
            parameterGradientInfoLength = lenWeights + outputNRows;

            //allocate contiguous memory for weights, biases, and gradients
            parameters = (double*) calloc(parameterLength, sizeof(double));
            parameterGradientInfo = (double*) calloc(parameterGradientInfoLength, sizeof(double));

            weights = new Matrix(parameters, outputNRows, inputNRows * inputNCols, -1.0, 1.0);
            weightGradient = new Matrix(parameterGradientInfo, outputNRows, inputNRows * inputNCols);

            weightUpdater = new MomentumUpdater(networkInformation, outputNRows, inputNRows * inputNCols);
            biasUpdater = new MomentumUpdater(networkInformation, outputNRows, 1);

            // weightUpdater = new RMSUpdater(networkInformation, outputNRows, inputNRows);
            // biasUpdater = new RMSUpdater(networkInformation, outputNRows, 1);

            // weightUpdater = new AdamUpdater(networkInformation, outputNRows, inputNRows);
            // biasUpdater = new AdamUpdater(networkInformation, outputNRows, 1);

            biases = new Matrix(&(parameters[lenWeights]), outputNRows, 1, -1.0, 1.0);
            biasGradient = new Matrix(&(parameterGradientInfo[lenWeights]), outputNRows, 1, -1.0, 1.0);

            outputInfoLength = outputNRows;
            outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

            output = new Matrix(outputInfo, outputInfoLength, 1);

            if(index != 0) {
                input = new Matrix(layers[index - 1]->getOutputInfo(), inputNRows * inputNCols, 1);

                layerGradientInfoLength = layers[index - 1]->getOutputInfoLength();
                layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

                layerGradient = new Matrix(layerGradientInfo, inputNRows * inputNCols, 1);
            }
        }

        void postInitialize() {
            if(index != networkInformation->getAmountOfLayers() - 1)
                error = new Matrix(layers[index + 1]->getLayerGradientInfo(), outputNRows, 1);
        }

        Matrix* feedForward() {
            weights->multiply(input, output);
            output->mAdd(biases);

            return output;
        }

        Matrix* calculateGradient() {
            if(index != 0)
                weights->multiplyInputTransposed(error, layerGradient);

            if(networkInformation->getBatchIndex() == 0) {
                error->multiplyOtherTransposed(input, weightGradient);
                biasGradient->copy(error);
            } else {
                error->multiplyOtherTransposedAdded(input, weightGradient, weightGradient);
                biasGradient->mAdd(error);
            }

            return layerGradient;
        }

        void update() {
            weightUpdater->update(weights, weightGradient);
            biasUpdater->update(biases, biasGradient);
        }
};