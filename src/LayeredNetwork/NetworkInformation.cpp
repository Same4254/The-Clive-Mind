#include "LayeredNetwork/NetworkInformation.hpp"

NetworkInformation::NetworkInformation(std::vector<std::unique_ptr<Layer>>& layers) : layers(layers) {
    learningRate = 0.001;
    velocityCoefficient = 0.9;
    batchSize = 1;
    batchIndex = 0;
}

NetworkInformation::~NetworkInformation() { }

int NetworkInformation::getAmountOfLayers() { return layers.size(); }
std::vector<std::unique_ptr<Layer>>& NetworkInformation::getLayers() { return layers; }

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