#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "Matrix.hpp"
#include "LayeredNetwork/NetworkInformation.hpp"

//Source: https://ruder.io/optimizing-gradient-descent/index.html#adam
class Updater {
protected:
    NetworkInformation& networkInformation;
    int parameterRows, parameterCols;

    double* learningData;
    int learningDataLength;

public:
    Updater(NetworkInformation& networkInformation, int parameterRows, int parameterCols);
    virtual ~Updater();

    void writeStateToFile(FILE* file);
    bool loadStateFromFile(FILE* file);

    virtual void update(Matrix* parameters, Matrix* gradient) = 0;

    double* getLearningData();
    int getLearningDataLength();
};
#endif