#include "LayeredNetwork/Layers/FullyConnectedLayer.hpp"

FullyConnectedLayer::FullyConnectedLayer(NetworkInformation& networkInformation, UpdaterID updaterID, int index, int numNodes) : Layer(networkInformation, Full, index), updaterID(updaterID) {
    outputMatrixCount = 1;
    outputNCols = 1;
    outputNRows = numNodes;
}

FullyConnectedLayer::~FullyConnectedLayer() {
    //Parameter and Gradients pointers are cleared by the Layer class. Just de-refrence them
    weights->setData(NULL);
    biases->setData(NULL);

    delete weights;
    delete biases;

    weightGradient->setData(NULL);
    biasGradient->setData(NULL);

    delete weightGradient;
    delete biasGradient;
}

void FullyConnectedLayer::initialize() {
    int lenWeights = (outputNRows * inputNRows * inputNCols);
    parameterLength = lenWeights + outputNRows;
    parameterGradientInfoLength = lenWeights + outputNRows;

    //allocate contiguous memory for weights, biases, and gradients
    parameters = (double*) calloc(parameterLength, sizeof(double));
    parameterGradientInfo = (double*) calloc(parameterGradientInfoLength, sizeof(double));

    weights = new Matrix(parameters, outputNRows, inputNRows * inputNCols, -1.0, 1.0);
    weightGradient = new Matrix(parameterGradientInfo, outputNRows, inputNRows * inputNCols);

    weightUpdater = createUpdaterFromID(updaterID, networkInformation, outputNRows, inputNRows * inputNCols);
    biasUpdater = createUpdaterFromID(updaterID, networkInformation, outputNRows, 1);

    biases = new Matrix(&(parameters[lenWeights]), outputNRows, 1, -1.0, 1.0);
    biasGradient = new Matrix(&(parameterGradientInfo[lenWeights]), outputNRows, 1, -1.0, 1.0);

    outputInfoLength = outputNRows;
    outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

    output = new Matrix(outputInfo, outputInfoLength, 1);

    if(index != 0) {
        input = new Matrix(networkInformation.getLayers()[index - 1]->getOutputInfo(), inputNRows * inputNCols, 1);

        layerGradientInfoLength = networkInformation.getLayers()[index - 1]->getOutputInfoLength();
        layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

        layerGradient = new Matrix(layerGradientInfo, inputNRows * inputNCols, 1);
    }
}

void FullyConnectedLayer::postInitialize() {
    if(index != networkInformation.getAmountOfLayers() - 1)
        error = new Matrix(networkInformation.getLayers()[index + 1]->getLayerGradientInfo(), outputNRows, 1);
    else
        error = new Matrix(outputNRows, 1);
}

void FullyConnectedLayer::writeStructureToFile(rapidjson::Value& layerJSONObject, rapidjson::Document::AllocatorType& allocator) {
    layerJSONObject.AddMember("Name", "Full", allocator);

    rapidjson::Value propertiesJSONObject;
    propertiesJSONObject.SetObject();

    propertiesJSONObject.AddMember("Updater", (int) updaterID, allocator);
    propertiesJSONObject.AddMember("NumNodes", outputNRows, allocator);

    layerJSONObject.AddMember("Properties", propertiesJSONObject, allocator);
}

void FullyConnectedLayer::writeStateToFile(FILE* file) {
    weightUpdater->writeStateToFile(file);
    biasUpdater->writeStateToFile(file);
}

bool FullyConnectedLayer::loadStateFromFile(FILE* file) {
    if(!weightUpdater->loadStateFromFile(file) || !biasUpdater->loadStateFromFile(file)) {
        std::cerr << "Fully Connected Layer could not load state from file" << std::endl;
        return false;
    }

    return true;
}

Matrix* FullyConnectedLayer::feedForward() {
    weights->multiply(input, output);
    output->mAdd(biases);

    return output;
}

Matrix* FullyConnectedLayer::calculateGradient() {
    if(index != 0)
        weights->multiplyInputTransposed(error, layerGradient);

    if(networkInformation.getBatchIndex() == 0) {
        error->multiplyOtherTransposed(input, weightGradient);
        biasGradient->copyData(error);
    } else {
        error->multiplyOtherTransposedAdded(input, weightGradient, weightGradient);
        biasGradient->mAdd(error);
    }

    return layerGradient;
}

void FullyConnectedLayer::update() {
    weightUpdater->update(weights, weightGradient);
    biasUpdater->update(biases, biasGradient);
}