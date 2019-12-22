#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "Updater.hpp"

#endif

class MomentumUpdater : public Updater {
    private:
        Matrix* velocity;

    public:
        MomentumUpdater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) : Updater(networkInformation, parameterRows, parameterCols) {
            this->velocity = new Matrix(parameterRows, parameterCols);
        }

        void update(Matrix* parameter, Matrix* gradient) {
            velocity->mScale(networkInformation->getVelocityCoefficient());
            velocity->mSubtract(gradient);
            parameter->mAdd(velocity);
        }
};