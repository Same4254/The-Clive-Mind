#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "Updater.hpp"

#endif

class MomentumUpdater : public Updater {
    private:
        Matrix* velocity;
        int parameterCount;

        double velocityCoefficient = 0.9;
        double learningRate = 0.01;

    public:
        MomentumUpdater(int parameterRows, int parameterCols) {
            this->velocity = new Matrix(parameterRows, parameterCols);
        }

        void update(Matrix* parameter, Matrix* gradient) {
            velocity->mScale(velocityCoefficient);
            velocity->mSubtract(gradient);
            parameter->mAdd(velocity);
        }


        // MomentumUpdater(int parameterCount, int parameterRows, int parameterCols) {
        //     this->parameterCount = parameterCount;

        //     velocity = (Matrix*) malloc(sizeof(Matrix) * parameterCount);
        //     for(int i = 0; i < parameterCount; i++)
        //         new (&velocity[i]) Matrix(parameterRows, parameterCols);
        // }

        // void update(Matrix* parameter, Matrix* gradient) {
        //     for(int i = 0; i < parameterCount; i++) {
        //         (&velocity[i])->mScale(velocityCoefficient);
        //         (&velocity[i])->mSubtract(&gradient[i]);
        //         (&parameter[i])->mAdd(&velocity[i]);
        //     }
        // }
};