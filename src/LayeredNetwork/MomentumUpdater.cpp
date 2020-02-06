#include "LayeredNetwork/MomentumUpdater.hpp"

MomentumUpdater::MomentumUpdater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) : Updater(networkInformation, parameterRows, parameterCols) {
    this->velocity = new Matrix(parameterRows, parameterCols);
}

MomentumUpdater::~MomentumUpdater() {
    delete velocity;
}

void MomentumUpdater::update(Matrix* parameter, Matrix* gradient) {
    gradient->mScale(networkInformation->getLearningRate());
    velocity->mScale(networkInformation->getVelocityCoefficient());
    velocity->mSubtract(gradient);
    parameter->mAdd(velocity);
}