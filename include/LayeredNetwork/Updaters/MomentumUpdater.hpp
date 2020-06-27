#ifndef MOMENTUM_UPDATER_HPP
#define MOMENTUM_UPDATER_HPP

#include "LayeredNetwork/Updaters/Updater.hpp"

class MomentumUpdater : public Updater {
private:
    Matrix* velocity;

public:
    MomentumUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols);
    ~MomentumUpdater();

    void update(Matrix* parameter, Matrix* gradient);
};
#endif