#include "LayeredNetwork/Updater.hpp"

Updater::Updater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) {
    this->networkInformation = networkInformation;
    this->parameterRows = parameterRows;
    this->parameterCols = parameterCols;

    learningData = NULL;
    learningDataLength = 0;
}

Updater::~Updater() {
    free(learningData);
}

double* Updater::getLearningData() { return learningData; }
int Updater::getLearningDataLength() { return learningDataLength; }