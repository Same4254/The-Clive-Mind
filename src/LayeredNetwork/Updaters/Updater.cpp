#include "LayeredNetwork/Updaters/Updater.hpp"

Updater::Updater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : networkInformation(networkInformation), parameterRows(parameterRows), parameterCols(parameterCols) {

    learningData = NULL;
}

Updater::~Updater() {
    free(learningData);
}

void Updater::writeStateToFile(FILE* file) {
    if(learningDataLength > 0) 
        fwrite(learningData, sizeof(double), learningDataLength, file);
}

bool Updater::loadStateFromFile(FILE* file) {
    if(learningDataLength > 0) {
        if(fread(learningData, sizeof(double), learningDataLength, file) != learningDataLength) {
            std::cerr << "Learning Data of file is not valid!" << std::endl;
            return false;
        }
    }

    return true;
}

double* Updater::getLearningData() { return learningData; }
int Updater::getLearningDataLength() { return learningDataLength; }