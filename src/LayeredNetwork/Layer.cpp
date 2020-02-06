// #include "include/LayeredNetwork/Layer.hpp"
#include "LayeredNetwork/Layer.hpp"

Layer::Layer(NetworkInformation* networkInformation, Layer** layers, int index) {
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

    input = NULL;
    output = NULL;
    layerGradient = NULL;
    error = NULL;

    inputMatrixCount = 0;
    inputNRows = 0;
    inputNCols = 0;

    outputMatrixCount = 0;
    outputNRows = 0;
    outputNCols = 0;
}

Layer::~Layer() {
    if(input != NULL) {
        for(int i = 0; i < inputMatrixCount; i++) {
            input[i].setData(NULL);
            delete &input[i];
        }    
    }

    if(layerGradient != NULL) {
        for(int i = 0; i < inputMatrixCount; i++) {
            layerGradient[i].setData(NULL);
            delete &layerGradient[i];
        }
    }

    if(output != NULL) {
        for(int i = 0; i < outputMatrixCount; i++) {
            output[i].setData(NULL);
            delete &output[i];   
        }
    }

    if(error != NULL) {
        for(int i = 0; i < outputMatrixCount; i++) {
            error[i].setData(NULL);
            delete &error[i];
        }
    }

    // free(parameters);
    // free(parameterGradientInfo);
    // free(layerGradientInfo);
    // free(outputInfo);
}

void Layer::initialize() {
    if(index != 0) {
        inputMatrixCount = layers[index - 1]->outputMatrixCount;
        inputNCols = layers[index - 1]->outputNCols;
        inputNRows = layers[index - 1]->outputNRows;
    }
}

int Layer::getParameterLength() { return parameterLength; }
double* Layer::getParameters() { return parameters; }

int Layer::getParameterGradientInfoLength() { return parameterGradientInfoLength; }
double* Layer::getParameterGradientInfo() { return parameterGradientInfo; }

Matrix* Layer::getLayerGradient() { return layerGradient; }
int Layer::getLayerGradientInfoLength() { return layerGradientInfoLength; }
double* Layer::getLayerGradientInfo() { return layerGradientInfo; }

int Layer::getInputMatrixCount() { return inputMatrixCount; }
int Layer::getInputNRows() { return inputNRows; }
int Layer::getInputNCols() { return inputNCols; }

int Layer::getOutputMatrixCount() { return outputMatrixCount; }
int Layer::getOutputNRows() { return outputNRows; }
int Layer::getOutputNCols() { return outputNCols; }

void Layer::setInputMatrix(Matrix* input) { this->input = input; }
void Layer::setInputMatrixCount(int inputMatrixCount) { this->inputMatrixCount = inputMatrixCount; }
void Layer::setInputNRows(int inputNRows) { this->inputNRows = inputNRows; }
void Layer::setInputNCols(int inputNCols) { this->inputNCols = inputNCols; }

void Layer::setOutputMatrixCount(int outputMatrixCount) { this->outputMatrixCount = outputMatrixCount; }
void Layer::setOutputNRows(int outputNRows) { this->outputNRows = outputNRows; }
void Layer::setOutputNCols(int outputNCols) { this->outputNCols = outputNCols; }

Matrix* Layer::getError() { return error; }
void Layer::setError(Matrix* error) { this->error = error; }

Matrix* Layer::getOutput() { return output; }
double* Layer::getOutputInfo() { return outputInfo; }
int Layer::getOutputInfoLength() { return outputInfoLength; }