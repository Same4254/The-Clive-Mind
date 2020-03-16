#ifndef LAYER_HPP
#define LAYER_HPP

#include <stdio.h>
#include <memory>

#include "Matrix.hpp"
#include "LayeredNetwork/NetworkInformation.hpp"

#include "LayeredNetwork/Updaters/Updater.hpp"
#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"
#include "LayeredNetwork/Updaters/AdamUpdater.hpp"
#include "LayeredNetwork/Updaters/RMSUpdater.hpp"

enum LayerID {
    Full, Conv, Act, Pool
};

enum UpdaterID {
    Momentum, Adam, RMS
};

enum ActivationID {
    Sigmoid, Relu, Softplus, LeakyRelu
};

class Layer {
protected:
    int inputMatrixCount;
    int inputNRows;
    int inputNCols;

    int outputMatrixCount;
    int outputNRows;
    int outputNCols;

    int parameterLength;
    double* parameters;

    int parameterGradientInfoLength;
    double* parameterGradientInfo;

    int layerGradientInfoLength;
    double* layerGradientInfo;

    int outputInfoLength;
    double* outputInfo;

    Matrix* input;
    Matrix* output;
    Matrix* layerGradient;
    Matrix* error;

    NetworkInformation& networkInformation;
    LayerID layerID;
    int index;

public:
    static std::unique_ptr<Updater> createUpdaterFromID(UpdaterID id, NetworkInformation& networkInformation, int parameterRows, int parameterCols);

    Layer(NetworkInformation& networkInformation, LayerID layerID, int index);
    virtual ~Layer();
    virtual void initialize();
    virtual void postInitialize() = 0;

    virtual void writeConstructInfo(FILE* file);
    virtual void writeState(FILE* file);
    virtual bool loadState(FILE* file);

    virtual Matrix* feedForward() = 0;
    virtual Matrix* calculateGradient() = 0;
    virtual void update() = 0;

    int getParameterLength();
    double* getParameters();

    int getParameterGradientInfoLength();
    double* getParameterGradientInfo();

    Matrix* getLayerGradient();
    int getLayerGradientInfoLength();
    double* getLayerGradientInfo();

    int getInputMatrixCount();
    int getInputNRows();
    int getInputNCols();

    int getOutputMatrixCount();
    int getOutputNRows();
    int getOutputNCols();

    void setInputMatrix(Matrix* input);
    void setInputMatrixCount(int inputMatrixCount);
    void setInputNRows(int inputNRows);
    void setInputNCols(int inputNCols);

    void setOutputMatrixCount(int outputMatrixCount);
    void setOutputNRows(int outputNRows);
    void setOutputNCols(int outputNCols);

    LayerID getLayerID();

    Matrix* getError();

    Matrix* getOutput();
    double* getOutputInfo();
    int getOutputInfoLength();
};
#endif