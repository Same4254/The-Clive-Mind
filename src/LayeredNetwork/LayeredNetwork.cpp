#include "LayeredNetwork/LayeredNetwork.hpp"

LayeredNetwork::LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols) : networkInformation(layers), inputMatrixCount(inputMatrixCount), inputNRows(inputNRows), inputNCols(inputNCols) {
    
}

LayeredNetwork::LayeredNetwork(LayeredNetwork* structureNetwork) : networkInformation(layers, structureNetwork->networkInformation), 
        inputMatrixCount(structureNetwork->inputMatrixCount), inputNRows(structureNetwork->inputNRows), inputNCols(structureNetwork->inputNCols),
        outputMatrixCount(structureNetwork->outputMatrixCount), outputNRows(structureNetwork->outputNRows), outputNCols(structureNetwork->outputNCols) {
    
    for(unsigned int i = 0; i < structureNetwork->networkInformation.getLayers().size(); i++)
        structureNetwork->networkInformation.getLayers()[i]->appendCopy(networkInformation);

    initialize();
}

LayeredNetwork::~LayeredNetwork() {
    if(layers.size() > 0)
        layers[0]->setInputMatrix(NULL);

    for(int i = 0; i < inputMatrixCount; i++) {
        (&inputMatrix[i])->setData(NULL);
        delete (&inputMatrix[i]);
    }

    for(int i = 0; i < outputMatrixCount; i++) {
        (&labelMatrix[i])->setData(NULL);
        delete (&labelMatrix[i]);
    }
}

void LayeredNetwork::initialize() { 
    layers[0]->setInputMatrixCount(inputMatrixCount);
    layers[0]->setInputNRows(inputNRows);
    layers[0]->setInputNCols(inputNCols);

    for(size_t i = 0; i < layers.size(); i++) {
        layers[i]->Layer::initialize();
        layers[i]->initialize();
    }

    for(size_t i = 0; i < layers.size(); i++)
        layers[i]->postInitialize();

    outputMatrixCount = layers[layers.size() - 1]->getOutputMatrixCount();
    outputNRows = layers[layers.size() - 1]->getOutputNRows();
    outputNCols = layers[layers.size() - 1]->getOutputNCols();

    inputMatrix = (Matrix*) malloc(inputMatrixCount * sizeof(Matrix));
    labelMatrix = (Matrix*) malloc(outputMatrixCount * sizeof(Matrix));

    /**
     * The idea here is that all of the input matricies will simply point to the input recourse
     * Rather than copying the data into memory. Thus, the matricies start with NULL as their 
     * data pointer. When an input is provided, this input matricies will point to the provided data
     * without having to copy or re-arrange themselves since the matrix knows the amount of rows and cols
     * it takes up.
     * 
     * This is a very similar story with the output matricies
     */
    for(int i = 0; i < inputMatrixCount; i++)
        new (&inputMatrix[i]) Matrix(NULL, inputNRows, inputNCols);

    for(int i = 0; i < outputMatrixCount; i++)
        new (&labelMatrix[i]) Matrix(NULL, outputNRows, outputNCols);

    //tells the first layer where to look for input
    layers[0]->setInputMatrix(inputMatrix);
}

void LayeredNetwork::writeStructureToFile(std::string filename) {
    //Build the JSON
    rapidjson::Document document;
    document.SetObject();

    document.AddMember("NumberInputMatricies", inputMatrixCount, document.GetAllocator());
    document.AddMember("NumberInputRows", inputNRows, document.GetAllocator());
    document.AddMember("NumberInputCols", inputNCols, document.GetAllocator());

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value layerArray(rapidjson::kArrayType);

    for(int i = 0; i < networkInformation.getAmountOfLayers(); i++) {
        rapidjson::Value layerObject;
        layerObject.SetObject();

        layers[i]->writeStructureToFile(layerObject, allocator);
        
        layerArray.PushBack(layerObject, allocator);
    }

    document.AddMember("Layers", layerArray, allocator);

    //Output to File
    std::ofstream outputFileStream(filename);
    rapidjson::OStreamWrapper outputFileStreamWrapper(outputFileStream);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(outputFileStreamWrapper);
    document.Accept(writer);
}

void LayeredNetwork::writeStateToFile(std::string filename) {
    FILE* stateFile = fopen(filename.c_str(), "w");
    for(int i = 0; i < networkInformation.getAmountOfLayers(); i++) {
        layers[i]->Layer::writeStateToFile(stateFile);
        layers[i]->writeStateToFile(stateFile);
    }

    fclose(stateFile);
}

void LayeredNetwork::loadStateFromFile(std::string filename) {
    FILE* stateFile = fopen(filename.c_str(), "r");
    for(int i = 0; i < networkInformation.getAmountOfLayers(); i++) {
        layers[i]->Layer::loadStateFromFile(stateFile);
        layers[i]->loadStateFromFile(stateFile);
    }

    fclose(stateFile);
}

bool LayeredNetwork::isEqualArchitecture(LayeredNetwork* otherNetwork) {
    if(inputMatrixCount != otherNetwork->inputMatrixCount || inputNRows != otherNetwork->inputNRows || inputNCols != otherNetwork->inputNCols
            || outputMatrixCount != otherNetwork->outputMatrixCount || outputNRows != otherNetwork->outputNRows || outputNCols != otherNetwork->outputNCols) {
        std::cout << "Models have different dimensions!" << std::endl;
        std::cout << inputNRows << ", " << inputNCols << ", " << inputMatrixCount << std::endl;
        std::cout << outputNRows << ", " << outputNCols << ", " << outputMatrixCount << std::endl;
        std::cout << otherNetwork->inputNRows << ", " << otherNetwork->inputNCols << ", " << otherNetwork->inputMatrixCount << std::endl;
        std::cout << otherNetwork->outputNRows << ", " << otherNetwork->outputNCols << ", " << otherNetwork->outputMatrixCount << std::endl;

        return false;
    }

    if(otherNetwork->networkInformation.getAmountOfLayers() != networkInformation.getAmountOfLayers()) {
        std::cout << "Models have different amount of layers!" << std::endl;
        return false;
    }

    for(int i = 0; i < networkInformation.getAmountOfLayers(); i++)
        if(!layers[i]->isEqualLayerArchitecture(otherNetwork->layers[i]))
            return false;

    return true;
}

void LayeredNetwork::copyState(LayeredNetwork* otherNetwork) {
    if(!isEqualArchitecture(otherNetwork)) {
        std::cout << "Cannot copy the state of a model with a different architecture!" << std::endl;
        return;
    }

    for(int i = 0; i < networkInformation.getAmountOfLayers(); i++)
        layers[i]->copyState(otherNetwork->layers[i]);
}

void LayeredNetwork::trainEpoch(Database* database) {
    for(int i = 0; i < database->getEpochSize(); i++) {
        double* trainingData = database->getTrainingData(i);
        double* trainingLabel = database->getTrainingLabel(i);

        feedForward(trainingData);
        calculateSuprivisedGradients(trainingLabel);

        if(networkInformation.incrementBatchIndex())
            update();
    }
}

Matrix* LayeredNetwork::feedForward(double* input) {
    int index = 0;
    for(int i = 0; i < inputMatrixCount; i++) {
        (&inputMatrix[i])->setData(&input[index]);
        index += (&inputMatrix[i])->getLength();
    }

    for(unsigned int i = 0; i < layers.size(); i++)
        layers[i]->feedForward();

    return getOutput();
}

void LayeredNetwork::calculateSuprivisedGradients(double* labels) {
    int index = 0;
    for(int i = 0; i < inputMatrixCount; i++) {
        (&labelMatrix[i])->setData(&labels[index]);
        index += (&labelMatrix[i])->getLength();
    }

    Matrix* output = getOutput();
    Matrix* error = layers[layers.size() - 1]->getError();

    for(int i = 0; i < outputMatrixCount; i++)
        (&output[i])->subtract(&labelMatrix[i], &error[i]);

    for(int i = layers.size() - 1; i >= 0; i--)
        layers[i]->calculateGradient();
}

void LayeredNetwork::addGradient(double loss, unsigned int index) {
    Matrix* error = layers[layers.size() - 1]->getError();

    error->clear();
    error->getData()[index] = loss;

    //TODO make this better... very inefficient
    for(int i = layers.size() - 1; i >= 0; i--)
        layers[i]->calculateGradient();
}

void LayeredNetwork::update() {
    for(unsigned int i = 0; i < layers.size(); i++)
        layers[i]->update();
}

double LayeredNetwork::evaluate(Database* database) {
    double error = 0;

    for(int index = 0; index < database->getTestSize(); index++) {
        double* testData = database->getTestData(index);
        double* testLabel = database->getTestLabel(index);

        int dataIndex = 0;
        for(int i = 0; i < outputMatrixCount; i++) {
            (&labelMatrix[i])->setData(&testLabel[dataIndex]);
            dataIndex += (&labelMatrix[i])->getLength();
        }

        feedForward(testData);

        for(int i = 0; i < layers[layers.size() - 1]->getOutputInfoLength(); i++) 
            error += pow(layers[layers.size() - 1]->getOutputInfo()[i] - testLabel[i], 2);
    }

    return error / (2.0 * database->getTestSize());
}

NetworkInformation& LayeredNetwork::getNetworkInformation() { return networkInformation; }
Matrix* LayeredNetwork::getOutput() { return layers[layers.size() - 1]->getOutput(); }

std::vector<std::unique_ptr<Layer>>& LayeredNetwork::getLayers() { return layers; }

int LayeredNetwork::getInputMatrixCount() { return inputMatrixCount; }
int LayeredNetwork::getInputNRows() { return inputNRows; }
int LayeredNetwork::getInputNCols() { return inputNCols; }

int LayeredNetwork::getOutputMatrixCount() { return outputMatrixCount; }
int LayeredNetwork::getOutputNRows() { return outputNRows; }
int LayeredNetwork::getOutputNCols() { return outputNCols; }