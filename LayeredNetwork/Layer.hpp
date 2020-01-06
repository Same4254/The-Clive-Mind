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

        int parameterGradientInfoLength;
        double* parameterGradientInfo;

        int layerGradientInfoLength;
        double* layerGradientInfo;

        int outputInfoLength;
        double* outputInfo;

        Matrix* input;
        Matrix* output;
        Matrix* layerGradient;
        Matrix* error;

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

            parameterGradientInfoLength = 0;
            parameterGradientInfo = NULL;

            outputInfoLength = 0;
            outputInfo = NULL;

            layerGradientInfoLength = 0;
            layerGradientInfo = NULL;
        }

        virtual ~Layer() {
            free(parameters);
            free(parameterGradientInfo);

            delete output;
            delete layerGradient;
        }

        virtual void initialize() {
            if(index != 0) {
                inputMatrixCount = layers[index - 1]->outputMatrixCount;
                inputNCols = layers[index - 1]->outputNCols;
                inputNRows = layers[index - 1]->outputNRows;
            }
        }

        virtual void postInitialize() = 0;
        virtual Matrix* feedForward() = 0;
        virtual Matrix* calculateGradient() = 0;
        virtual void update() = 0;

        int getParameterLength() { return parameterLength; }
        double* getParameters() { return parameters; }

        int getParameterGradientInfoLength() { return parameterGradientInfoLength; }
        double* getParameterGradientInfo() { return parameterGradientInfo; }

        Matrix* getLayerGradient() { return layerGradient; }
        int getLayerGradientInfoLength() { return layerGradientInfoLength; }
        double* getLayerGradientInfo() { return layerGradientInfo; }

        int getInputMatrixCount() { return inputMatrixCount; }
        int getInputNRows() { return inputNRows; }
        int getInputNCols() { return inputNCols; }

        int getOutputMatrixCount() { return outputMatrixCount; }
        int getOutputNRows() { return outputNRows; }
        int getOutputNCols() { return outputNCols; }

        void setInputMatrix(Matrix* input) { this->input = input; }
        void setInputMatrixCount(int inputMatrixCount) { this->inputMatrixCount = inputMatrixCount; }
        void setInputNRows(int inputNRows) { this->inputNRows = inputNRows; }
        void setInputNCols(int inputNCols) { this->inputNCols = inputNCols; }

        void setOutputMatrixCount(int outputMatrixCount) { this->outputMatrixCount = outputMatrixCount; }
        void setOutputNRows(int outputNRows) { this->outputNRows = outputNRows; }
        void setOutputNCols(int outputNCols) { this->outputNCols = outputNCols; }

        Matrix* getError() { return error; }
        void setError(Matrix* error) { this->error = error; }

        Matrix* getOutput() { return output; }
        double* getOutputInfo() { return outputInfo; }
        int getOutputInfoLength() { return outputInfoLength; }
};