#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "MomentumUpdater.hpp"

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

        Matrix* input;

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
            int lenWeights = (outputNRows * inputNRows);
            parameterLength = lenWeights + outputNRows;
            gradientLength = lenWeights + outputNRows;

            //allocate contiguous memory for weights, biases, and gradients
            parameters = (double*) calloc(parameterLength, sizeof(double));
            gradientInfo = (double*) calloc(gradientLength, sizeof(double));

            weights = new Matrix(&(parameters[0]), outputNRows, inputNRows, -1.0, 1.0);
            weightGradient = new Matrix(&(gradientInfo[0]), outputNRows, inputNRows);

            weightUpdater = new MomentumUpdater(networkInformation, outputNRows, inputNRows);
            biasUpdater = new MomentumUpdater(networkInformation, outputNRows, 1);

            biases = new Matrix(&(parameters[lenWeights]), outputNRows, 1, -1.0, 1.0);
            biasGradient = new Matrix(&(gradientInfo[lenWeights]), outputNRows, 1, -1.0, 1.0);

            output = new Matrix(outputNRows, 1);
            layerGradient = new Matrix(inputNRows, 1);
        }

        Matrix* feedForward(Matrix* input) {
            this->input = input;

            weights->multiply(input, output);
            output->mAdd(biases);

            return output;
        }

        Matrix* calculateGradient(Matrix* error) {
            if(index != 0)
                weights->multiplyInputTransposed(error, layerGradient);

            if(networkInformation->getBatchIndex() == 0)
                error->multiplyOtherTransposed(input, weightGradient);
            else
                error->multiplyOtherTransposedAdded(input, weightGradient, weightGradient);

            if(networkInformation->getBatchIndex() == networkInformation->getBatchSize() - 1) {
                weightGradient->mScale(networkInformation->getLearningRate());
                error->scale(networkInformation->getLearningRate(), biasGradient);
            }

            return layerGradient;
        }

        void update() {
            weightUpdater->update(weights, weightGradient);
            biasUpdater->update(biases, biasGradient);
        }
};