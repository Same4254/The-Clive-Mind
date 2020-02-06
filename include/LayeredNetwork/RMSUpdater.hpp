#ifndef RMS_UPDATER_HPP
#define RMS_UPDATER_HPP

#include "LayeredNetwork/Updater.hpp"

class RMSUpdater : public Updater {
private:
    Matrix* average;

    Matrix* squared;
    Matrix* root;
    Matrix* delta;

public:
    RMSUpdater(NetworkInformation* networkInformation, int parameterRows, int parameterCols);
    ~RMSUpdater();

    void update(Matrix* parameter, Matrix* gradient);
};
#endif