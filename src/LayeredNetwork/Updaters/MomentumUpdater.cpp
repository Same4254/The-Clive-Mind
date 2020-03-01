#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"

MomentumUpdater::MomentumUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : Updater(networkInformation, parameterRows, parameterCols), velocity(parameterRows, parameterCols) {

    learningData = velocity.getData();
    learningDataLength = velocity.getLength();
}

MomentumUpdater::~MomentumUpdater() { 
    learningData = NULL;
    learningDataLength = 0;
}

void MomentumUpdater::update(Matrix* parameter, Matrix* gradient) {
    gradient->mScale(networkInformation.getLearningRate());
    velocity.mScale(networkInformation.getVelocityCoefficient());
    velocity.mSubtract(gradient);
    parameter->mAdd(&velocity);
}