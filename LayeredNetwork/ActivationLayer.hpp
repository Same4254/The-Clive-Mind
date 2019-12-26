#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef ACTIVATION_FUNCTION_HPP
#define ACTIVATION_FUNCTION_HPP

#include "ActivationFunction.hpp"

#endif

class ActivationLayer: public Layer {
    private: 
        ActivationFunction* function;

    public:
        ActivationLayer(NetworkInformation* networkInformation, Layer** layers, int index, ActivationFunction* function) : Layer(networkInformation, layers, index) { 
            this->function = function;
        }

        ~ActivationLayer() {
        
        }

        void initialize() { 
            output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            layerGradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

            for(int i = 0; i < inputMatrixCount; i++) {
                new (&output[i]) Matrix(inputNRows, inputNCols);
                new (&layerGradient[i]) Matrix(inputNRows, inputNCols);
            }

            outputMatrixCount = inputMatrixCount;
            outputNRows = inputNRows;
            outputNCols = inputNCols;
        }

        Matrix* feedForward(Matrix* input) { 
            for(int i = 0; i < inputMatrixCount; i++) {
                function->applyFunction(&(input[i]), &(output[i]));
                function->applyDerivativeFunction(&(input[i]), &(layerGradient[i]));
            }

            return output;
        }

        Matrix* calculateGradient(Matrix* error) { 
            for(int i = 0; i < inputMatrixCount; i++)
                (&layerGradient[i])->elementProduct(&error[i], &layerGradient[i]);
            return layerGradient;
        }

        void update() {

        }
};