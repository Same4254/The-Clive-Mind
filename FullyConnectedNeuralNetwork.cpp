#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#include "./Matrix/Matrix.cpp"

using namespace std;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoidDerivative(double x) {
    return sigmoid(x) * (1.0 - sigmoid(x));
}

class FullyConnectedNeuralNetwork {
    public:
        int layers;
        int* layerSizes;
 
        double learningRate = 0.01;
        double velocityCoefficient = 0.9;

        double etaMinus = 0.5;
        double etaPlus = 1.2;
        double deltaMax = 50;
        double deltaMin = 1E-6;

        Matrix* weights;
        Matrix* biases;
        Matrix* activations;
        Matrix* nets;

        Matrix* answers;
        Matrix* error;
        Matrix* weightGradients;
        Matrix* biasGradients;

        Matrix* weightVelocities;
        Matrix* scaledWeightVelocities;
        Matrix* scaledScaledWeightVelocities;
        Matrix* weightDeltas;

        Matrix* biasVelocities;
        Matrix* scaledBiasVelocities;
        Matrix* scaledScaledBiasVelocities;
        Matrix* biasDeltas;

    public:
        FullyConnectedNeuralNetwork(int amountOfLayers, int* layerSizes) {
            this->layers = amountOfLayers;
            this->layerSizes = layerSizes;
            
            //forward information
            weights = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            biases = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            activations = (Matrix*) malloc(sizeof(Matrix) * layers);
            nets = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));

            //error terms
            answers = (Matrix*) malloc(sizeof(Matrix));
            error = (Matrix*) malloc(sizeof(Matrix) * (layers));
            weightGradients = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            biasGradients = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));

            //Momentum related terms
            weightVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            scaledWeightVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            scaledScaledWeightVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            weightDeltas = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            
            biasVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            scaledBiasVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            scaledScaledBiasVelocities = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            biasDeltas = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            
            new (&(answers[0])) Matrix(layerSizes[layers - 1], 1);

            for(int i = 1; i < layers; i++) {
                new (&(weights[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1], -1.0, 1.0);
                new (&(biases[i - 1])) Matrix(layerSizes[i], 1, -1.0, 1.0);

                new (&(error[i - 1])) Matrix(layerSizes[i], 1);
                new (&(nets[i - 1])) Matrix(layerSizes[i], 1);
                new (&(weightGradients[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1]);
                new (&(biasGradients[i - 1])) Matrix(layerSizes[i], 1);
                
                new (&(weightVelocities[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1]);
                new (&(scaledWeightVelocities[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1]);
                new (&(scaledScaledWeightVelocities[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1]);
                new (&(weightDeltas[i - 1])) Matrix(layerSizes[i], layerSizes[i - 1]);

                new (&(biasVelocities[i - 1])) Matrix(layerSizes[i], 1);
                new (&(scaledBiasVelocities[i - 1])) Matrix(layerSizes[i], 1);
                new (&(scaledScaledBiasVelocities[i - 1])) Matrix(layerSizes[i], 1);
                new (&(biasDeltas[i - 1])) Matrix(layerSizes[i], 1);
            }

            for(int i = 0; i < layers; i++)
                new (&(activations[i])) Matrix(layerSizes[i], 1);
        }

        Matrix* feedForward(double* input) {
            for(int i = 0; i < layerSizes[0]; i++) {
                (&activations[0])->set(i, 0, input[i]);
            }

            for(int i = 1; i < layers; i++) {
                (&weights[i - 1])->multiply(&activations[i - 1], &nets[i - 1])->mAdd(&biases[i - 1]);
                (&nets[i - 1])->forEach(&sigmoid, &activations[i]);
            }

            return &activations[layers - 1];
        }

        void backpropogate(double* answers) {
            nesterovBackpropogate(answers);
        }

        void nesterovBackpropogate(double* answers) {
            // for(int i = 0; i < layerSizes[layers - 1]; i++) {
            //     // this->answers->set(i, 0, answers[i]);
            //     this->answers->getData()[i] = answers[i];
            // }

            this->answers->setDataUNSAFE(answers);

            for(int weightLayer = layers - 2; weightLayer >= 0; weightLayer--) {
                if(weightLayer == layers - 2) {
                    (&activations[layers - 1])->subtract(this->answers, &error[weightLayer]);
                    (&nets[weightLayer])->forEach(&sigmoidDerivative, &nets[weightLayer]);
                    (&error[weightLayer])->elementProduct(&nets[weightLayer], &error[weightLayer]);
                } else {
                    (&weights[weightLayer + 1])->mTranspose();
                    (&weights[weightLayer + 1])->multiply(&(error[weightLayer + 1]), &(error[weightLayer]));
                    (&weights[weightLayer + 1])->mTranspose();

                    (&nets[weightLayer])->forEach(&sigmoidDerivative, &nets[weightLayer]);
                    (&error[weightLayer])->elementProduct(&nets[weightLayer], &error[weightLayer]);
                }

                (&activations[weightLayer])->mTranspose();
                (&error[weightLayer])->multiply(&activations[weightLayer], &weightGradients[weightLayer]);
                (&activations[weightLayer])->mTranspose();

                (&weightGradients[weightLayer])->mScale(learningRate);
                (&error[weightLayer])->scale(learningRate, &biasGradients[weightLayer]);

                //Momentum
                (&weightVelocities[weightLayer])->mScale(velocityCoefficient);
                (&weightVelocities[weightLayer])->mSubtract(&weightGradients[weightLayer]);
                (&weights[weightLayer])->mAdd(&weightVelocities[weightLayer]);

                (&biasVelocities[weightLayer])->mScale(velocityCoefficient);
                (&biasVelocities[weightLayer])->mSubtract(&biasGradients[weightLayer]);
                (&biases[weightLayer])->mAdd(&biasVelocities[weightLayer]);

                // Nesterov
                // (&weightVelocities[weightLayer])->scale(velocityCoefficient, &scaledWeightVelocities[weightLayer]);
                // (&scaledWeightVelocities[weightLayer])->subtract(&weightGradients[weightLayer], &weightVelocities[weightLayer]);
                // (&weightVelocities[weightLayer])->scale(1.0 + velocityCoefficient, &scaledScaledWeightVelocities[weightLayer]);
                // (&scaledScaledWeightVelocities[weightLayer])->subtract(&scaledWeightVelocities[weightLayer], &weightDeltas[weightLayer]);
                // (&weights[weightLayer])->mAdd(&weightDeltas[weightLayer]);

                // (&biasVelocities[weightLayer])->scale(velocityCoefficient, &scaledBiasVelocities[weightLayer]);
                // (&scaledBiasVelocities[weightLayer])->subtract(&biasGradients[weightLayer], &biasVelocities[weightLayer]);
                // (&biasVelocities[weightLayer])->scale(1.0 + velocityCoefficient, &scaledScaledBiasVelocities[weightLayer]);
                // (&scaledScaledBiasVelocities[weightLayer])->subtract(&scaledBiasVelocities[weightLayer], &biasDeltas[weightLayer]);
                // (&biases[weightLayer])->mAdd(&biasDeltas[weightLayer]);
            }
        }
};
