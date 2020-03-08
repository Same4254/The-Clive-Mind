#include "LayeredNetwork/LayeredNetwork.hpp"

LayeredNetwork::LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols) : networkInformation(layers), inputMatrixCount(inputMatrixCount), inputNRows(inputNRows), inputNCols(inputNCols) {
    
}

LayeredNetwork::~LayeredNetwork() {
    if(layers.size() > 0)
        layers[0]->setInputMatrix(NULL);

    for(int i = 0; i < inputMatrixCount; i++) {
        (&inputMatrix[i])->setData(NULL);
        delete (&inputMatrix[i]);
    }

    for(int i = 0; i < outputMatrixCount; i++) {
        (&labelMatrix[i])->setData(NULL);
        delete (&labelMatrix[i]);
    }
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

    inputMatrix = (Matrix*) malloc(inputMatrixCount * sizeof(Matrix));
    labelMatrix = (Matrix*) malloc(outputMatrixCount * sizeof(Matrix));

    for(int i = 0; i < inputMatrixCount; i++)
        new (&inputMatrix[i]) Matrix(NULL, inputNRows, inputNCols);

    for(int i = 0; i < outputMatrixCount; i++)
        new (&labelMatrix[i]) Matrix(NULL, outputNRows, outputNCols);

    layers[0]->setInputMatrix(inputMatrix);
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

void LayeredNetwork::trainEpoch(Database* database) {
    for(int i = 0; i < database->getEpochSize(); i++) {
        double* trainingData = database->getTrainingData(i);
        double* trainingLabel = database->getTrainingLabel(i);

        int index = 0;
        for(int i = 0; i < inputMatrixCount; i++) {
            (&inputMatrix[i])->setData(&trainingData[index]);
            index += (&inputMatrix[i])->getLength();
        }

        index = 0;
        for(int i = 0; i < outputMatrixCount; i++) {
            (&labelMatrix[i])->setData(&trainingLabel[index]);
            index += (&labelMatrix[i])->getLength();
        }

        for(int i = 0; i < layers.size(); i++)
            layers[i]->feedForward();

        calculateGradients();

        if(networkInformation.incrementBatchIndex())
            update();
    }
}

Matrix* LayeredNetwork::feedForward(double* input) {
    int index = 0;
    for(int i = 0; i < inputMatrixCount; i++) {
        (&inputMatrix[i])->setData(&input[index]);
        index += (&inputMatrix[i])->getLength();
    }

    for(int i = 0; i < layers.size(); i++)
        layers[i]->feedForward();

    return getOutput();
}

void LayeredNetwork::calculateGradients() {
    Matrix* output = getOutput();
    Matrix* error = layers[layers.size() - 1]->getError();

    for(int i = 0; i < outputMatrixCount; i++)
        (&output[i])->subtract(&labelMatrix[i], &error[i]);

    for(int i = layers.size() - 1; i >= 0; i--)
        layers[i]->calculateGradient();
}

void LayeredNetwork::calculateGradients(double* labels) {
    int index = 0;
    for(int i = 0; i < inputMatrixCount; i++) {
        (&labelMatrix[i])->setData(&labels[index]);
        index += (&labelMatrix[i])->getLength();
    }

    calculateGradients();
}

void LayeredNetwork::update() {
    for(int i = 0; i < layers.size(); i++)
        layers[i]->update();
}

double LayeredNetwork::evaluate(Database* database) {
    double right = 0;

    for(int index = 0; index < database->getTestSize(); index++) {
        double* testData = database->getTestData(index);
        double* testLabel = database->getTestLabel(index);

        int dataIndex = 0;
        for(int i = 0; i < inputMatrixCount; i++) {
            (&inputMatrix[i])->setData(&testData[dataIndex]);
            dataIndex += (&inputMatrix[i])->getLength();
        }

        dataIndex = 0;
        for(int i = 0; i < outputMatrixCount; i++) {
            (&labelMatrix[i])->setData(&testLabel[dataIndex]);
            dataIndex += (&labelMatrix[i])->getLength();
        }

        for(int i = 0; i < layers.size(); i++)
            layers[i]->feedForward();

        int greatestOutputIndex = 0;
        for(int i = 0; i < layers[layers.size() - 1]->getOutputInfoLength(); i++) {
            if(layers[layers.size() - 1]->getOutputInfo()[i] > layers[layers.size() - 1]->getOutputInfo()[greatestOutputIndex])
                greatestOutputIndex = i;
        }

        int greatestLabelIndex = 0;
        for(int i = 0; i < layers[layers.size() - 1]->getOutputInfoLength(); i++) {
            if(testLabel[i] > testLabel[greatestLabelIndex])
                greatestLabelIndex = i;
        }

        if(greatestOutputIndex == greatestLabelIndex)
            right++;
    }

    return right / database->getTestSize();
}

NetworkInformation& LayeredNetwork::getNetworkInformation() { return networkInformation; }
Matrix* LayeredNetwork::getOutput() { return layers[layers.size() - 1]->getOutput(); }

std::vector<std::unique_ptr<Layer>>& LayeredNetwork::getLayers() { return layers; }