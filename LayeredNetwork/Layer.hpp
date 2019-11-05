#ifndef ACTIVATION_FUNCTION_HPP
#define ACTIVATION_FUNCTION_HPP

#include "ActivationFunction.hpp"

#endif

class Layer {
    protected:
        int inputMatrixCount;
        int inputNRows;
        int inputNCols;

        int outputMatrixCount;
        int outputNRows;
        int outputNCols;

        Matrix* output;
        Matrix* gradient;

        Layer** networkLayers;
        int index;

    public:
        Layer(Layer** networkLayers, int index) {
            this->networkLayers = networkLayers;
            this->index = index;
        }

        virtual void initialize() {
            if(index != 0) {
                inputMatrixCount = networkLayers[index - 1]->outputMatrixCount;
                inputNCols = networkLayers[index - 1]->outputNCols;
                inputNRows = networkLayers[index - 1]->outputNRows;
            }
        }

        virtual Matrix* feedForward(Matrix* input) = 0;
        virtual Matrix* backpropogate(Matrix* error) = 0;

        int getInputMatrixCount() { return inputMatrixCount; }
        int getInputNRows() { return inputNRows; }
        int getInputNCols() { return inputNCols; }

        int getOutputMatrixCount() { return outputMatrixCount; }
        int getOutputNRows() { return outputNRows; }
        int getOutputNCols() { return outputNCols; }

        void setInputMatrixCount(int inputMatrixCount) { this->inputMatrixCount = inputMatrixCount; }
        void setInputNRows(int inputNRows) { this->inputNRows = inputNRows; }
        void setInputNCols(int inputNCols) { this->inputNCols = inputNCols; }

        void setOutputMatrixCount(int outputMatrixCount) { this->outputMatrixCount = outputMatrixCount; }
        void setOutputNRows(int outputNRows) { this->outputNRows = outputNRows; }
        void setOutputNCols(int outputNCols) { this->outputNCols = outputNCols; }

        Matrix* getOutput() { return output; }
        Matrix* getGradient() { return gradient; }
};