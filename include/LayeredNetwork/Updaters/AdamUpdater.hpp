#ifndef ADAM_UPDATER_HPP
#define ADAM_UPDATER_HPP

#include "LayeredNetwork/Updaters/Updater.hpp"

class AdamUpdater : public Updater {
private:
    Matrix* average;
    Matrix* average2;

    Matrix* squared;
    Matrix* root;
    Matrix* scaled;
    Matrix* delta;

public:
    AdamUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols);
    ~AdamUpdater();

    void update(Matrix* parameter, Matrix* gradient);
};
#endif