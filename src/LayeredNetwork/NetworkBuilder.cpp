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

LayeredNetwork* NetworkBuilder::fromFile(std::string filename) {
    FILE* file = fopen(filename.c_str(), "r");

    int info[4];

    if(fread(info, sizeof(int), 4, file) != 4) {
        return NULL;
    }

    int amountOfLayers = info[0];

    network = std::make_unique<LayeredNetwork>(info[1], info[2], info[3]);

    for(int i = 0; i < amountOfLayers; i++) {
        int id;
        if(fread(&id, sizeof(int), 1, file) != 1) {
            network.reset();
            return NULL;
        }

        LayerID layer = (LayerID) id;

        if(layer == Full) {
            int up;

            if(fread(&up, sizeof(int), 1, file) != 1) {
                network.reset();
                return NULL;
            }

            UpdaterID updater = (UpdaterID) up;

            int num;
            if(fread(&num, sizeof(int), 1, file) != 1) {
                network.reset();
                return NULL;
            }

            fullyConnectedLayer(updater, num);
        } else if(layer == Act) {
            int func;
            if(fread(&func, sizeof(int), 1, file) != 1) {
                network.reset();
                return NULL;
            }

            activationLayer((ActivationID) func);
        } else if(layer == Conv) {
            int data[4];

            if(fread(data, sizeof(int), 4, file) != 4) {
                network.reset();
                return NULL;
            }

            convolutionLayer((UpdaterID) data[0], data[1], data[2], data[3]);
        }
    }

    network->initialize();

    for(int i = 0; i < amountOfLayers; i++) {
        if(!network->getLayers()[i]->loadState(file)) {
            network.reset();
            return NULL;
        }

        if(!network->getLayers()[i]->Layer::loadState(file)) {
            network.reset();
            return NULL;
        }
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