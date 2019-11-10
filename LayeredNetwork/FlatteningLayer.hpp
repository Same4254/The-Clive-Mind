#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

class FlatteningLayer : public Layer {
    public:
        FlatteningLayer(Layer** layers, int index) : Layer(layers, index) {

        }

        void initialize() {
            outputMatrixCount = 1;
            outputNCols = 1;
            outputNRows = inputNRows *inputNCols * inputMatrixCount;

            output = new Matrix(outputNRows, outputNCols);
            
            gradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            for(int i = 0; i < inputMatrixCount; i++) {
                new (&gradient[i]) Matrix(inputNRows, inputNCols);
            }
        }

        Matrix* feedForward(Matrix* input) {
            if(inputMatrixCount == 1 && inputNCols == 1)//already flat
                return input;//This should really never happen
            
            int index = 0;
            for(int matrix = 0; matrix < inputMatrixCount; matrix++) {
                for(int i = 0; i < inputNRows * inputNCols; i++) {
                    output->getData()[index] = (&input[matrix])->getData()[i];

                    index++;
                }
            }

            return output;
        }

        Matrix* backpropogate(Matrix* error) {
            int index = 0;
            for(int matrix = 0; matrix < inputMatrixCount; matrix++) {
                for(int i = 0; i < inputNRows * inputNCols; i++) {
                    (&gradient[matrix])->getData()[i] = error->getData()[index];

                    index++;
                }
            }

            return gradient;
        }
};