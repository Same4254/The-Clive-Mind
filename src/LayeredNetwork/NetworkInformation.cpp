#include "LayeredNetwork/NetworkInformation.hpp"

NetworkInformation::NetworkInformation(int amountOfLayers) {
    this->amountOfLayers = amountOfLayers;

    learningRate = 0.0;
    velocityCoefficient = 0.0;
    batchSize = 1;
    batchIndex = 0;
}

NetworkInformation::~NetworkInformation() {
    
}

int NetworkInformation::getAmountOfLayers() { return amountOfLayers; }

double NetworkInformation::getLearningRate() { return learningRate; }
void NetworkInformation::setLearningRate(double learningRate) { this->learningRate = learningRate; }

double NetworkInformation::getVelocityCoefficient() { return velocityCoefficient; }
void NetworkInformation::setVelocityCoefficient(double velocityCoefficient) { this->velocityCoefficient = velocityCoefficient; }

int NetworkInformation::getBatchSize() { return batchSize; }
void NetworkInformation::setBatchSize(int batchSize) { this->batchSize = batchSize; }

bool NetworkInformation::incrementBatchIndex() {
    batchIndex++;

    if(batchIndex == batchSize) {
        batchIndex = 0;
        return true;
    }

    return false;
}

int NetworkInformation::getBatchIndex() { return batchIndex; }