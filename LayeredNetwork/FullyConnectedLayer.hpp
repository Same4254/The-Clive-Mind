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

        double learningRate;

    public:
        FullyConnectedLayer(Layer** networkLayers, int index, int numNodes) : Layer(networkLayers, index) {
            outputMatrixCount = 1;
            outputNCols = 1;
            outputNRows = numNodes;
        }

        void initialize() {
            weights = new Matrix(outputNRows, inputNRows, -1.0, 1.0);
            weightGradient = new Matrix(outputNRows, inputNRows);

            weightUpdater = new MomentumUpdater(outputNRows, inputNRows);
            biasUpdater = new MomentumUpdater(outputNRows, 1);

            activationFunction = new SigmoidFunction();

            biases = new Matrix(outputNRows, 1, -1.0, 1.0);
            biasGradient = new Matrix(outputNRows, 1, -1.0, 1.0);

            output = new Matrix(outputNRows, 1);
            nets = new Matrix(outputNRows, 1);

            gradient = new Matrix(inputNRows, 1);
        }

        Matrix* feedForward(Matrix* input) {
            this->input = input;

            weights->multiply(input, nets);
            nets->mAdd(biases);
            activationFunction->applyFunction(nets, output);

            return output;
        }

        Matrix* backpropogate(Matrix* error) {
            activationFunction->applyDerivativeFunction(nets);
            error->elementProduct(nets, error);

            //Weight Gradient Calculation
            input->mTranspose();
            error->multiply(input, weightGradient);
            input->mTranspose();

            //Parameter Update
            weightGradient->mScale(0.01);
            error->scale(0.01, biasGradient);

            weightUpdater->update(weights, weightGradient);
            biasUpdater->update(biases, biasGradient);

            //Layer Gradient Calculation
            weights->mTranspose();
            weights->multiply(error, gradient);
            weights->mTranspose();

            return gradient;
        }
};