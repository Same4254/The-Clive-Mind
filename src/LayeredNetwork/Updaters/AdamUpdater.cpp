#include "LayeredNetwork/Updaters/AdamUpdater.hpp"

AdamUpdater::AdamUpdater(NetworkInformation& networkInformation, int parameterRows, int parameterCols) : Updater(networkInformation, parameterRows, parameterCols) {
    average = new Matrix(parameterRows, parameterCols);
    average2 = new Matrix(parameterRows, parameterCols);

    squared = new Matrix(parameterRows, parameterCols);
    scaled = new Matrix(parameterRows, parameterCols);
    root = new Matrix(parameterRows, parameterCols);
    delta = new Matrix(parameterRows, parameterCols);
}

AdamUpdater::~AdamUpdater() {
    delete average;
    delete average2;

    delete scaled;
    delete squared;
    delete root;
    delete delta;
}

void AdamUpdater::update(Matrix* parameter, Matrix* gradient) {
    double b1 = 0.9;
    double b2 = 0.99;
    double epsilon = 1e-10;

    //New Average
    gradient->pow(2, squared);
    
    average->mScale(b1);//need to be saved in learningData
    average2->mScale(b2);

    gradient->scale(1.0 - b1, scaled);
    squared->mScale(1.0 - b2);

    average->mAdd(scaled);
    average2->mAdd(squared);

    //Delta
    average->scale(networkInformation.getLearningRate(), gradient);//Place holder
    average2->pow(.5, root);
    root->mAdd(epsilon);

    gradient->elementDivision(root, delta);

    parameter->mSubtract(delta);
}