#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "Updater.hpp"

#endif

class RMSUpdater : public Updater {
    private:
        Matrix* average;

        Matrix* squared;
        Matrix* root;
        Matrix* delta;

    public:
        RMSUpdater(NetworkInformation* networkInformation, int parameterRows, int parameterCols) : Updater(networkInformation, parameterRows, parameterCols) {
            average = new Matrix(parameterRows, parameterCols);
            squared = new Matrix(parameterRows, parameterCols);
            root = new Matrix(parameterRows, parameterCols);
            delta = new Matrix(parameterRows, parameterCols);
        }

        ~RMSUpdater() {
            delete average;
            delete squared;
            delete root;
            delete delta;
        }

        void update(Matrix* parameter, Matrix* gradient) {
            double p = 0.9;
            double epsilon = 1e-10;

            //New Average
            average->mScale(p);
            gradient->pow(2, squared);
            squared->mScale(1 - p);
            average->mAdd(squared);

            //Delta
            gradient->mScale(-networkInformation->getLearningRate());
            average->pow(.5, root);
            root->mAdd(epsilon);

            gradient->elementDivision(root, delta);

            parameter->mAdd(delta);
        }
};