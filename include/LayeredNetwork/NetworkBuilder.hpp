#ifndef NETWORK_BUILDER
#define NETWORK_BUILDER

#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

#include "LayeredNetwork/LayeredNetwork.hpp"
#include "LayeredNetwork/Layers/FullyConnectedLayer.hpp"
#include "LayeredNetwork/Layers/ConvolutionalLayer.hpp"
#include "LayeredNetwork/Layers/ActivationLayer.hpp"
#include "LayeredNetwork/ActivationFunctions/SigmoidFunction.hpp"

class NetworkBuilder {
private:
    std::unique_ptr<LayeredNetwork> network;
    int inputMatrixCount, inputNRows, inputNCols;

    void createNetwork();

public:
    NetworkBuilder();
    ~NetworkBuilder();

    LayeredNetwork* build();
    LayeredNetwork* loadStructurefromFile(std::string filename);

    NetworkBuilder& fullyConnectedLayer(UpdaterID id, int numNodes);
    NetworkBuilder& convolutionLayer(UpdaterID id, int kernalCount, int kernalSize, int stride);
    NetworkBuilder& activationLayer(ActivationID id);

private:
    LayeredNetwork* loadStructurefromJSONDocument(rapidjson::Document& document);
    void fullyConnectedLayer(rapidjson::Value::ConstObject &documentObject);
    void convolutionLayer(rapidjson::Value::ConstObject &documentObject);
    void activationLayer(rapidjson::Value::ConstObject &documentObject);

public:
    void matrixCount(int inputMatrixCount) { this->inputMatrixCount = inputMatrixCount; }
    void inputRows(int inputNRows) { this->inputNRows = inputNRows; }
    void inputCols(int inputNCols) { this->inputNCols = inputNCols; }
};
#endif