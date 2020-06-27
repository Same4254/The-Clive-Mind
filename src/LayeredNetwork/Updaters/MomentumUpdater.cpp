#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"

MomentumUpdater::MomentumUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) 
    : Updater(networkInformation, parameterRows, parameterCols) {

    learningData = (double*) calloc(parameterRows * parameterCols, sizeof(double));
    velocity = new Matrix(learningData, parameterRows, parameterCols);
    learningDataLength = velocity->getLength();
}

MomentumUpdater::~MomentumUpdater() {
    velocity->setData(NULL);
    delete velocity;
}

void MomentumUpdater::update(Matrix* parameter, Matrix* gradient) {
    gradient->mScale(networkInformation.getLearningRate());
    velocity->mScale(networkInformation.getVelocityCoefficient());
    velocity->mSubtract(gradient);
    parameter->mAdd(velocity);
}