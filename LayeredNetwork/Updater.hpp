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

    public:
        Updater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) {
            this->networkInformation = networkInformation;
            this->parameterRows = parameterRows;
            this->parameterCols = parameterCols;
        }

        virtual void update(Matrix* parameters, Matrix* gradient) = 0;
};