#include "LayeredNetwork/NetworkBuilder.hpp"

NetworkBuilder::NetworkBuilder() {
    inputMatrixCount = 1;//Default values
    inputNRows = 1;
    inputNCols = 1;
}

NetworkBuilder::~NetworkBuilder() { }

void NetworkBuilder::createNetwork() {
    network = std::make_unique<LayeredNetwork>(inputMatrixCount, inputNRows, inputNCols);
}

LayeredNetwork* NetworkBuilder::build() {
    network->initialize();
    return network.release();
}

LayeredNetwork* NetworkBuilder::loadStructurefromJSONDocument(rapidjson::Document& document) {
    inputMatrixCount = document["NumberInputMatricies"].GetInt();
    inputNRows = document["NumberInputRows"].GetInt();
    inputNCols = document["NumberInputCols"].GetInt();

    rapidjson::Value::Array layerArray = document["Layers"].GetArray();

    for (rapidjson::Value::ConstValueIterator itr = layerArray.Begin(); itr != layerArray.End(); ++itr) {
        rapidjson::Value::ConstObject layer = itr->GetObject();

        std::string name = layer["Name"].GetString();
        rapidjson::Value::ConstObject properties = layer["Properties"].GetObject();
        
        if(name == "Full")
            fullyConnectedLayer(properties);
        else if(name == "Conv")
            convolutionLayer(properties);
        else if(name == "Activation")
            activationLayer(properties);
    }
    
    return build();
}

LayeredNetwork* NetworkBuilder::loadStructurefromFile(std::string filename) {
    std::ifstream inputFileStream(filename);
    rapidjson::IStreamWrapper inputFileStreamWrapper(inputFileStream);

    rapidjson::Document document;
    document.ParseStream(inputFileStreamWrapper);

    inputFileStream.close();

    return loadStructurefromJSONDocument(document);
}

/***************** Fully Connected Layer Parser *****************/
NetworkBuilder& NetworkBuilder::fullyConnectedLayer(UpdaterID id, int numNodes) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<FullyConnectedLayer>(network->getNetworkInformation(), id, network->getLayers().size(), numNodes));
    return *this;
}

void NetworkBuilder::fullyConnectedLayer(rapidjson::Value::ConstObject &documentObject) {
    int updaterID = documentObject["Updater"].GetInt();
    int numNodes = documentObject["NumNodes"].GetInt();

    fullyConnectedLayer((UpdaterID) updaterID, numNodes);
}

/***************** Convolutional Layer Parser *****************/
NetworkBuilder& NetworkBuilder::convolutionLayer(UpdaterID id, int kernalCount, int kernalSize, int stride) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<ConvolutionalLayer>(network->getNetworkInformation(), id, network->getLayers().size(), kernalCount, kernalSize, stride));
    return *this;
}

void NetworkBuilder::convolutionLayer(rapidjson::Value::ConstObject &documentObject) {
    int updaterID = documentObject["Updater"].GetInt();
    int numKernals = documentObject["NumKernals"].GetInt();
    int kernalSize = documentObject["KernalSize"].GetInt();
    int stride = documentObject["Stride"].GetInt();

    convolutionLayer((UpdaterID) updaterID, numKernals, kernalSize, stride);
}

/***************** Activation Layer Parser *****************/
NetworkBuilder& NetworkBuilder::activationLayer(ActivationID id) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<ActivationLayer>(network->getNetworkInformation(), id, network->getLayers().size()));
    return *this;
}

void NetworkBuilder::activationLayer(rapidjson::Value::ConstObject &documentObject) {
    int activationID = documentObject["ActivationID"].GetInt();

    activationLayer((ActivationID) activationID);
}