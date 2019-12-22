#ifndef MATRIX_HPP

#define MATRIX_HPP
#include "../Matrix/Matrix.hpp"

#endif

#ifndef NETWORK_INFORMATION_CPP
#define NETWORK_INFORMATION_CPP

#include "NetworkInformation.cpp"

#endif

class Layer {
    protected:
        int inputMatrixCount;
        int inputNRows;
        int inputNCols;

        int outputMatrixCount;
        int outputNRows;
        int outputNCols;

        int parameterLength;
        double* parameters;

        int gradientLength;
        double* gradientInfo;

        Matrix* output;
        Matrix* layerGradient;

        NetworkInformation* networkInformation;
        Layer** layers;
        int index;

    public:
        Layer(NetworkInformation* networkInformation, Layer** layers, int index) {
            this->networkInformation = networkInformation;
            this->layers = layers;
            this->index = index;

            parameterLength = 0;
            parameters = NULL;

            gradientLength = 0;
            gradientInfo = NULL;
        }

        virtual void initialize() {
            if(index != 0) {
                inputMatrixCount = layers[index - 1]->outputMatrixCount;
                inputNCols = layers[index - 1]->outputNCols;
                inputNRows = layers[index - 1]->outputNRows;
            }
        }

        virtual Matrix* feedForward(Matrix* input) = 0;
        virtual Matrix* calculateGradient(Matrix* error) = 0;
        virtual void update() = 0;

        int getParameterLength() { return parameterLength; }
        double* getParameters() { return parameters; }

        int getGradientLength() { return gradientLength; }
        double* getGradientInfo() { return gradientInfo; }

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
        Matrix* getGradient() { return layerGradient; }
};