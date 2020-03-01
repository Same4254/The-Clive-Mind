#include "LayeredNetwork/Updaters/RMSUpdater.hpp"

RMSUpdater::RMSUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) : Updater(networkInformation, parameterRows, parameterCols) {
    average = new Matrix(parameterRows, parameterCols);
    squared = new Matrix(parameterRows, parameterCols);
    root = new Matrix(parameterRows, parameterCols);
    delta = new Matrix(parameterRows, parameterCols);
}

RMSUpdater::~RMSUpdater() {
    delete average;
    delete squared;
    delete root;
    delete delta;
}

void RMSUpdater::update(Matrix* parameter, Matrix* gradient) {
    double p = 0.9;
    double epsilon = 1e-10;

    //New Average
    average->mScale(p);//needs to be in LearningData pointer
    gradient->pow(2, squared);
    squared->mScale(1 - p);
    average->mAdd(squared);

    //Delta
    gradient->mScale(-networkInformation.getLearningRate());
    average->pow(.5, root);
    root->mAdd(epsilon);

    gradient->elementDivision(root, delta);

    parameter->mAdd(delta);
}