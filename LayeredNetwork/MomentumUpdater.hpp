#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "Updater.hpp"

#endif

class MomentumUpdater : public Updater {
    private:
        Matrix* velocity;
        double velocityCoefficient = 0.9;
        double learningRate = 0.01;

    public:
        MomentumUpdater(int parameterRows, int parameterCols) {
            velocity = new Matrix(parameterRows, parameterCols);
        }

        void update(Matrix* parameter, Matrix* gradient) {
            velocity->mScale(velocityCoefficient);
            velocity->mSubtract(gradient);
            parameter->mAdd(velocity);
        }
};