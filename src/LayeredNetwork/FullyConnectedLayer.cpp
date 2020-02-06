#include "LayeredNetwork/FullyConnectedLayer.hpp"

FullyConnectedLayer::FullyConnectedLayer(NetworkInformation* networkInformation, Layer** layers, int index, int numNodes) : Layer(networkInformation, layers, index) {
    outputMatrixCount = 1;
    outputNCols = 1;
    outputNRows = numNodes;
}

FullyConnectedLayer::~FullyConnectedLayer() {
    //Parameter and Gradients pointers are cleared by the Layer class. Just de-refrence the
    weights->setData(NULL);
    biases->setData(NULL);

    delete weights;
    delete biases;

    weightGradient->setData(NULL);
    biasGradient->setData(NULL);

    delete weightGradient;
    delete biasGradient;

    delete weightUpdater;
    delete biasUpdater;
}

void FullyConnectedLayer::initialize() {
    int lenWeights = (outputNRows * inputNRows * inputNCols);
    parameterLength = lenWeights + outputNRows;
    parameterGradientInfoLength = lenWeights + outputNRows;

    //allocate contiguous memory for weights, biases, and gradients
    parameters = (double*) calloc(parameterLength, sizeof(double));
    parameterGradientInfo = (double*) calloc(parameterGradientInfoLength, sizeof(double));

    weights = new Matrix(parameters, outputNRows, inputNRows * inputNCols, -1.0, 1.0);
    weightGradient = new Matrix(parameterGradientInfo, outputNRows, inputNRows * inputNCols);

    weightUpdater = new MomentumUpdater(networkInformation, outputNRows, inputNRows * inputNCols);
    biasUpdater = new MomentumUpdater(networkInformation, outputNRows, 1);

    // weightUpdater = new RMSUpdater(networkInformation, outputNRows, inputNRows);
    // biasUpdater = new RMSUpdater(networkInformation, outputNRows, 1);

    // weightUpdater = new AdamUpdater(networkInformation, outputNRows, inputNRows);
    // biasUpdater = new AdamUpdater(networkInformation, outputNRows, 1);

    biases = new Matrix(&(parameters[lenWeights]), outputNRows, 1, -1.0, 1.0);
    biasGradient = new Matrix(&(parameterGradientInfo[lenWeights]), outputNRows, 1, -1.0, 1.0);

    outputInfoLength = outputNRows;
    outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

    output = new Matrix(outputInfo, outputInfoLength, 1);

    if(index != 0) {
        input = new Matrix(layers[index - 1]->getOutputInfo(), inputNRows * inputNCols, 1);

        layerGradientInfoLength = layers[index - 1]->getOutputInfoLength();
        layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

        layerGradient = new Matrix(layerGradientInfo, inputNRows * inputNCols, 1);
    }
}

void FullyConnectedLayer::postInitialize() {
    if(index != networkInformation->getAmountOfLayers() - 1)
        error = new Matrix(layers[index + 1]->getLayerGradientInfo(), outputNRows, 1);
}

Matrix* FullyConnectedLayer::feedForward() {
    weights->multiply(input, output);
    output->mAdd(biases);

    return output;
}

Matrix* FullyConnectedLayer::calculateGradient() {
    if(index != 0)
        weights->multiplyInputTransposed(error, layerGradient);

    if(networkInformation->getBatchIndex() == 0) {
        error->multiplyOtherTransposed(input, weightGradient);
        biasGradient->copyData(error);
    } else {
        error->multiplyOtherTransposedAdded(input, weightGradient, weightGradient);
        biasGradient->mAdd(error);
    }

    return layerGradient;
}

void FullyConnectedLayer::update() {
    weightUpdater->update(weights, weightGradient);
    biasUpdater->update(biases, biasGradient);
}