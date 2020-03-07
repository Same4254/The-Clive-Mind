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

LayeredNetwork* NetworkBuilder::fromFile(char* filename) {
    FILE* file = fopen(filename, "r");

    int amountOfLayers, inputMatrixCount, inputNRows, inputNCols;

    fread(&amountOfLayers, sizeof(int), 1, file);
    fread(&inputMatrixCount, sizeof(int), 1, file);
    fread(&inputNRows, sizeof(int), 1, file);
    fread(&inputNCols, sizeof(int), 1, file);

    network = std::make_unique<LayeredNetwork>(inputMatrixCount, inputNRows, inputNCols);

    for(int i = 0; i < amountOfLayers; i++) {
        int id;
        fread(&id, sizeof(int), 1, file);

        LayerID layer = (LayerID) id;

        if(layer == Full) {
            int up;
            fread(&up, sizeof(int), 1, file);

            UpdaterID updater = (UpdaterID) up;

            int num;
            fread(&num, sizeof(int), 1, file);

            fullyConnectedLayer(updater, num);
        } else if(layer == Act) {
            int func;
            fread(&func, sizeof(int), 1, file);

            activationLayer((ActivationID) func);
        }
    }

    network->initialize();

    for(int i = 0; i < amountOfLayers; i++) {
        network->getLayers()[i]->loadState(file);
        network->getLayers()[i]->Layer::loadState(file);
    }

    fclose(file);
    return network.release();
}

NetworkBuilder& NetworkBuilder::fullyConnectedLayer(UpdaterID id, int numNodes) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<FullyConnectedLayer>(network->getNetworkInformation(), id, network->getLayers().size(), numNodes));
    return *this;
}

NetworkBuilder& NetworkBuilder::convolutionLayer(UpdaterID id, int kernalCount, int kernalSize, int stride) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<ConvolutionalLayer>(network->getNetworkInformation(), id, network->getLayers().size(), kernalCount, kernalSize, stride));
    return *this;
}

NetworkBuilder& NetworkBuilder::activationLayer(ActivationID id) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<ActivationLayer>(network->getNetworkInformation(), network->getLayers().size(), id));
    return *this;
}