#include <stdio.h>
#include <iostream>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"

int main() {
    NetworkBuilder builder;

    builder.inputRows(4);

    builder.fullyConnectedLayer(UpdaterID::Momentum, 10);
    LayeredNetwork* network = builder.build();

    network->toFile("./state");

    builder.fromFile("./state");
}