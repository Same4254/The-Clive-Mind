#include "LayeredNetwork/Updaters/Updater.hpp"

Updater::Updater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : networkInformation(networkInformation), parameterRows(parameterRows), parameterCols(parameterCols) {

    learningData = NULL;
    learningDataLength = 0;
}

Updater::~Updater() {
    free(learningData);
}

bool Updater::loadState(FILE* file) {
    if(learningDataLength > 0) {
        if(fread(learningData, sizeof(double), learningDataLength, file) != learningDataLength) {
            std::cerr << "Learning Data of file is not valid!" << std::endl;

            return false;
        }
    }

    return true;
}

void Updater::writeState(FILE* file) {
    if(learningDataLength > 0) 
        fwrite(learningData, sizeof(double), learningDataLength, file);
}

double* Updater::getLearningData() { return learningData; }
int Updater::getLearningDataLength() { return learningDataLength; }