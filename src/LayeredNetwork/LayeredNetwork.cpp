#include "LayeredNetwork/LayeredNetwork.hpp"

LayeredNetwork::LayeredNetwork(int amountOfLayers, int inputMatrixCount, int inputNRows, int inputNCols, int outputMatrixCount, int outputNRows, int outputNCols) { 
    this->amountOfLayers = amountOfLayers;

    this->inputMatrixCount = inputMatrixCount;
    this->inputNRows = inputNRows;
    this->inputNCols = inputNCols;

    this->outputMatrixCount = outputMatrixCount;
    this->outputNRows = outputNRows;
    this->outputNCols = outputNCols;

    layers = new Layer*[amountOfLayers];

    error = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
    for(int i = 0; i < outputMatrixCount; i++)
        new (&error[i]) Matrix(outputNRows, outputNCols);

    networkInformation = new NetworkInformation(amountOfLayers);
}

LayeredNetwork::~LayeredNetwork() {
    if(amountOfLayers > 0) {
        layers[amountOfLayers - 1]->setError(NULL);
        layers[0]->setInputMatrix(NULL);
    }

    for(int i = 0; i < amountOfLayers; i++)
        delete layers[i];
    delete[] layers;

    for(int i = 0; i < outputMatrixCount; i++)
        delete &error[i];

    delete networkInformation;
}

void LayeredNetwork::initialize() { 
    layers[0]->setInputMatrixCount(inputMatrixCount);
    layers[0]->setInputNRows(inputNRows);
    layers[0]->setInputNCols(inputNCols);

    layers[amountOfLayers - 1]->setOutputMatrixCount(outputMatrixCount);
    layers[amountOfLayers - 1]->setOutputNRows(outputNRows);
    layers[amountOfLayers - 1]->setOutputNCols(outputNCols);

    for(int i = 0; i < amountOfLayers; i++) {
        layers[i]->Layer::initialize();
        layers[i]->initialize();
    }

    for(int i = 0; i < amountOfLayers; i++) {
        layers[i]->postInitialize();
    }

    layers[amountOfLayers - 1]->setError(error);
}

Matrix* LayeredNetwork::feedForward(Matrix* input) {
    layers[0]->setInputMatrix(input);

    for(int i = 0; i < amountOfLayers; i++) {
        if(i == 0) 
            layers[0]->feedForward();
        else
            layers[i]->feedForward();
    }

    return layers[amountOfLayers - 1]->getOutput();
}

void LayeredNetwork::calculateGradients(Matrix* labels) {
    for(int i = 0; i < outputMatrixCount; i++)
        (&getOutput()[i])->subtract((&labels[i]), &error[i]);

    for(int i = amountOfLayers - 1; i >= 0; i--) {
        if(i == amountOfLayers - 1)
            layers[i]->calculateGradient();
        else 
            layers[i]->calculateGradient();
    }
}

void LayeredNetwork::update() {
    for(int i = 0; i < amountOfLayers; i++)
        layers[i]->update();
}

void LayeredNetwork::writeState(FILE* fd) {
    
}

NetworkInformation* LayeredNetwork::getNetworkInformation() { return networkInformation; }
Matrix* LayeredNetwork::getOutput() { return layers[amountOfLayers - 1]->getOutput(); }

int LayeredNetwork::getAmountOfLayers() { return amountOfLayers; }
Layer** LayeredNetwork::getLayers() { return layers; }