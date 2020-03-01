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

NetworkBuilder& NetworkBuilder::fullyConnectedLayer(UpdaterID id, int numNodes) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<FullyConnectedLayer>(network->getNetworkInformation(), id, network->getLayers().size(), numNodes));
    return *this;
}

NetworkBuilder& NetworkBuilder::activationLayer(ActivationID id) {
    if(network == nullptr)
        createNetwork();

    network->getLayers().push_back(std::make_unique<ActivationLayer>(network->getNetworkInformation(), network->getLayers().size(), id));
    return *this;
}