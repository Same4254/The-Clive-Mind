#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#ifndef SIGMOID_FUNCTION_HPP
#define SIGMOID_FUNCTION_HPP

#include "SigmoidFunction.hpp"

#endif

#ifndef RELU_FUNCTION_HPP
#define RELU_FUNCTION_HPP

#include "ReluFunction.hpp"

#endif

class ActivationLayer: public Layer {
    private: 
        ActivationFunction* function;

    public:
        ActivationLayer(Layer** networkLayers, int index, ActivationFunction* function) : Layer(networkLayers, index) { 
            this->function = function;
        }

        void initialize() { 
            output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            gradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

            for(int i = 0; i < inputMatrixCount; i++) {
                new (&output[i]) Matrix(inputNRows, inputNCols);
                new (&gradient[i]) Matrix(inputNRows, inputNCols);
            }

            outputMatrixCount = inputMatrixCount;
            outputNRows = inputNRows;
            outputNCols = inputNCols;
        }

        Matrix* feedForward(Matrix* input) { 
            for(int i = 0; i < inputMatrixCount; i++) {
                function->applyFunction(&(input[i]), &(output[i]));
                function->applyDerivativeFunction(&(input[i]), &(gradient[i]));
            }

            return output;
        }

        Matrix* backpropogate(Matrix* error) { 
            for(int i = 0; i < inputMatrixCount; i++)
                (&gradient[i])->elementProduct(&error[i], &gradient[i]);
            return gradient;
        }
};