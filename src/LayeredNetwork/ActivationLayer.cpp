#include "LayeredNetwork/ActivationLayer.hpp"

ActivationLayer::ActivationLayer(NetworkInformation* networkInformation, Layer** layers, int index, ActivationFunction* function) : Layer(networkInformation, layers, index) { 
    this->function = function;
}

ActivationLayer::~ActivationLayer() {
    delete function;
}

void ActivationLayer::initialize() {
    outputMatrixCount = inputMatrixCount;
    outputNRows = inputNRows;
    outputNCols = inputNCols;

    outputInfoLength = outputNRows * outputNCols * outputMatrixCount;
    outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

    layerGradientInfoLength = inputNRows * inputNCols * inputMatrixCount;
    layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

    output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
    layerGradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

    for(int i = 0; i < inputMatrixCount; i++) {
        new (&output[i]) Matrix(&outputInfo[i * outputNRows * outputNCols], inputNRows, inputNCols);
        new (&layerGradient[i]) Matrix(&layerGradientInfo[i * inputNRows * inputNCols], inputNRows, inputNCols);
    }

    if(index != 0) {
        input = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

        for(int i = 0; i < inputMatrixCount; i++) {
            new (&input[i]) Matrix(&layers[index - 1]->getOutputInfo()[inputNCols * inputNRows * i], inputNRows, inputNCols);
        }
    }
}

void ActivationLayer::postInitialize() {
    if(index != networkInformation->getAmountOfLayers() - 1) {
        error = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
        for(int i = 0; i < outputMatrixCount; i++)
            new (&error[i]) Matrix(&layers[index + 1]->getLayerGradientInfo()[outputNRows * outputNCols * i], outputNRows, outputNCols);
    }
}

Matrix* ActivationLayer::feedForward() { 
    for(int i = 0; i < inputMatrixCount; i++) {
        function->applyFunction(&(input[i]), &(output[i]));
        function->applyDerivativeFunction(&(input[i]), &(layerGradient[i]));
    }

    return output;
}

Matrix* ActivationLayer::calculateGradient() { 
    for(int i = 0; i < inputMatrixCount; i++)
        (&layerGradient[i])->elementProduct(&error[i], &layerGradient[i]);
    return layerGradient;
}