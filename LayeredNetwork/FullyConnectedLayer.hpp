#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "MomentumUpdater.hpp"

#endif

#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "SigmoidFunction.hpp"

#endif

class FullyConnectedLayer : public Layer {
    private:
        Matrix* weights;
        Matrix* biases;

        Matrix* nets;
        Matrix* weightGradient;
        Matrix* biasGradient;

        Matrix* input;

        Updater* weightUpdater;
        Updater* biasUpdater;

        ActivationFunction* activationFunction;

    public:
        FullyConnectedLayer(NetworkInformation* networkInformation, Layer** layers, int index, int numNodes) : Layer(networkInformation, layers, index) {
            outputMatrixCount = 1;
            outputNCols = 1;
            outputNRows = numNodes;
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

            activationFunction = new SigmoidFunction();

            biases = new Matrix(&(parameters[lenWeights]), outputNRows, 1, -1.0, 1.0);
            biasGradient = new Matrix(&(gradientInfo[lenWeights]), outputNRows, 1, -1.0, 1.0);

            output = new Matrix(outputNRows, 1);
            nets = new Matrix(outputNRows, 1);

            layerGradient = new Matrix(inputNRows, 1);
        }

        Matrix* feedForward(Matrix* input) {
            this->input = input;

            weights->multiply(input, nets);
            nets->mAdd(biases);
            activationFunction->applyFunction(nets, output);

            return output;
        }

        Matrix* calculateGradient(Matrix* error) {
            activationFunction->applyDerivativeFunction(nets);
            error->elementProduct(nets, error);

            // //Layer Gradient Calculation
            // weights->mTranspose();
            // weights->multiply(error, layerGradient);
            // weights->mTranspose();

            // //Weight Gradient Calculation
            // input->mTranspose();
            // error->multiply(input, weightGradient);
            // input->mTranspose();

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