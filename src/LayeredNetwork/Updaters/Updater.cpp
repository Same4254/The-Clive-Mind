#include "LayeredNetwork/Updaters/Updater.hpp"

Updater::Updater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : networkInformation(networkInformation), parameterRows(parameterRows), parameterCols(parameterCols) {

    learningData = NULL;
    learningDataLength = 0;
}

Updater::~Updater() {
    free(learningData);
}

void Updater::loadState(FILE* file) {
    if(learningDataLength > 0) 
        fread(learningData, sizeof(double), learningDataLength, file);
}

void Updater::toFile(FILE* file) {
    if(learningDataLength > 0) 
        fwrite(learningData, sizeof(double), learningDataLength, file);
}

double* Updater::getLearningData() { return learningData; }
int Updater::getLearningDataLength() { return learningDataLength; }