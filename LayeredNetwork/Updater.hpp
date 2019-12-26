#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../Matrix/Matrix.hpp"

#endif

#ifndef NETWORK_INFORMATION_CPP
#define NETWORK_INFORMATION_CPP

#include "NetworkInformation.cpp"

#endif

class Updater {
    protected:
        NetworkInformation* networkInformation;
        int parameterRows, parameterCols;

        double* learningData;
        int learningDataLength;

    public:
        Updater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) {
            this->networkInformation = networkInformation;
            this->parameterRows = parameterRows;
            this->parameterCols = parameterCols;

            learningData = NULL;
            learningDataLength = 0;
        }

        virtual ~Updater() {
            free(learningData);
        }

        virtual void update(Matrix* parameters, Matrix* gradient) = 0;

        double* getLearningData() { return learningData; }
        int getLearningDataLength() { return learningDataLength; }
};