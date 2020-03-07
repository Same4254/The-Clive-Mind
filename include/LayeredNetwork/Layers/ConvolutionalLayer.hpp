#ifndef CONVOLUTIONAL_LAYER_HPP
#define CONVOLUTIONAL_LAYER_HPP

#include "LayeredNetwork/Layers/Layer.hpp"
#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"

class ConvolutionalLayer : public Layer {
private:
    Matrix** weights;
    Matrix** weightGradient;
    Updater** weightUpdater;

    Matrix* bias;
    Matrix* biasGradient;
    Updater* biasUpdater;

    UpdaterID updaterID;

    Matrix* errorDialatedPadded;

    int kernalSize;
    int stride;

    Matrix* convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped);
    Matrix* convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped, Matrix* adder);
    Matrix* convolute(Matrix* input, Matrix* filter, double bias, Matrix* output, int s, bool flipped, Matrix* adder);

public:
    ConvolutionalLayer(NetworkInformation& networkInformation, UpdaterID updaterID, int index, int kernalCount, int kernalSize, int stride);

    void initialize();
    void postInitialize();

    void toFile(FILE* file);
    void fromFile(FILE* file);

    Matrix* feedForward();
    Matrix* calculateGradient();

    void update();
};
#endif