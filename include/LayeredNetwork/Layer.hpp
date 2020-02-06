#ifndef LAYER_HPP
#define LAYER_HPP

#include "Matrix.hpp"
#include "LayeredNetwork/NetworkInformation.hpp"

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

    NetworkInformation* networkInformation;
    Layer** layers;
    int index;

public:
    Layer(NetworkInformation* networkInformation, Layer** layers, int index);

    virtual ~Layer();
    virtual void initialize();

    virtual void postInitialize() = 0;
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

    Matrix* getError();
    void setError(Matrix* error);

    Matrix* getOutput();
    double* getOutputInfo();
    int getOutputInfoLength();
};
#endif