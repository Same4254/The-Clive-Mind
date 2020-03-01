#include "LayeredNetwork/Updaters/Updater.hpp"

Updater::Updater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : networkInformation(networkInformation), parameterRows(parameterRows), parameterCols(parameterCols) {

    learningData = NULL;
    learningDataLength = 0;
}

Updater::~Updater() {
    free(learningData);
}

double* Updater::getLearningData() { return learningData; }
int Updater::getLearningDataLength() { return learningDataLength; }