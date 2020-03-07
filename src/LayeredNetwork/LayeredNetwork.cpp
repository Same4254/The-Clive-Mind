#include "LayeredNetwork/LayeredNetwork.hpp"

LayeredNetwork::LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols) : networkInformation(layers), inputMatrixCount(inputMatrixCount), inputNRows(inputNRows), inputNCols(inputNCols) {
    
}

LayeredNetwork::~LayeredNetwork() {
    if(layers.size() > 0)
        layers[0]->setInputMatrix(NULL);
}

void LayeredNetwork::initialize() { 
    layers[0]->setInputMatrixCount(inputMatrixCount);
    layers[0]->setInputNRows(inputNRows);
    layers[0]->setInputNCols(inputNCols);

    for(int i = 0; i < layers.size(); i++) {
        layers[i]->Layer::initialize();
        layers[i]->initialize();
    }

    for(int i = 0; i < layers.size(); i++)
        layers[i]->postInitialize();

    outputMatrixCount = layers[layers.size() - 1]->getOutputMatrixCount();
    outputNRows = layers[layers.size() - 1]->getOutputNRows();
    outputNCols = layers[layers.size() - 1]->getOutputNCols();
}

void LayeredNetwork::toFile(char* filename) {
    FILE* file = fopen(filename, "w");

    int amountOfLayers = layers.size();

    fwrite(&amountOfLayers, sizeof(int), 1, file);
    fwrite(&inputMatrixCount, sizeof(int), 1, file);
    fwrite(&inputNRows, sizeof(int), 1, file);
    fwrite(&inputNCols, sizeof(int), 1, file);

    for(int i = 0; i < amountOfLayers; i++) {
        int id = (int) layers[i]->getLayerID();

        fwrite(&id, sizeof(int), 1, file);
        layers[i]->writeConstructInfo(file);
    }

    for(int i = 0; i < amountOfLayers; i++) {
        layers[i]->writeState(file);
        layers[i]->Layer::writeState(file);
    }

    fclose(file);
}

Matrix* LayeredNetwork::feedForward(Matrix* input) {
    layers[0]->setInputMatrix(input);

    for(int i = 0; i < layers.size(); i++)
        layers[i]->feedForward();

    return getOutput();
}

void LayeredNetwork::calculateGradients(Matrix* labels) {
    Matrix* output = getOutput();
    Matrix* error = layers[layers.size() - 1]->getError();

    for(int i = 0; i < outputMatrixCount; i++)
        (&output[i])->subtract(&labels[i], &error[i]);

    for(int i = layers.size() - 1; i >= 0; i--)
        layers[i]->calculateGradient();
}

void LayeredNetwork::update() {
    for(int i = 0; i < layers.size(); i++)
        layers[i]->update();
}

NetworkInformation& LayeredNetwork::getNetworkInformation() { return networkInformation; }
Matrix* LayeredNetwork::getOutput() { return layers[layers.size() - 1]->getOutput(); }

std::vector<std::unique_ptr<Layer>>& LayeredNetwork::getLayers() { return layers; }