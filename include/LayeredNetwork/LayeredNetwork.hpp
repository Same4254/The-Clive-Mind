#ifndef LAYERED_NETWORK_HPP
#define LAYERED_NETWORK_HPP

#include <stdio.h>
#include <vector>
#include <math.h>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

#include "LayeredNetwork/NetworkInformation.hpp"
#include "LayeredNetwork/Layers/Layer.hpp"
#include "Databases/Database.hpp"

/**
 * The network is a series of layers that perform a series of matrix arithmetic, with the data flowing through one layer to the next.
 * In other words, the output of a layer is the input to the next layer. This is relfected in the code of the layers.
 * 
 * The general flow is that a given layer has a pointer to the output of the previous layer. This is such that there is NO copying of data as it flows through the network.
 * This mentality of using pointers to prevent copying is used throughout the code.
 * While this purposes some *interesting* pointer lifetime control, it is heavily an optimization decision that makes the network crunch more updates.
 * 
 * Networks should be constructed through the NetworkBuilder class, since that makes the building process easy.
 */
class LayeredNetwork {
private:
    NetworkInformation networkInformation;
    std::vector<std::unique_ptr<Layer>> layers;

    Matrix* inputMatrix;
    Matrix* labelMatrix;

    int inputMatrixCount;
    int inputNRows;
    int inputNCols;

    int outputMatrixCount;
    int outputNRows;
    int outputNCols;

public:
    LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols);
    LayeredNetwork(LayeredNetwork* structureNetwork);
    ~LayeredNetwork();

    void initialize();

    /**
     * Given the filename to a JSON file, this function will write this architecture
     *      to the file in JSON format such that it can be read back by the NetworkBuilder
     * This will NOT write the state of the network into the file.
     * 
     * If this file has anything in it already it will be removed in place of this data. 
     * 
     * @param filename -> The name of the file to write to.
     */
    void writeStructureToFile(std::string filename);

    /**
     * Takes the current state of the network and writes it to a file.
     * This is not a human readable format, rather just a dump of the bytes in the model's state.
     * A file containing this information can be read back into memory with loadStateToFile
     * 
     * If this file has anything in it already it will be removed in place of this data.
     * 
     * @param filename -> The name of the file to write to.
     */
    void writeStateToFile(std::string filename);

    /**
     * Given the filename to a state file, this will load the state into this network's memory.
     * The intent here is that states can be hot-swapped in and out of the architecture if desired.
     * This state includes learning information, weights, biases, etc...
     * 
     * This file does NOT contain information about the architecture, so be careful about loading a state 
     *      into a non-corresponding archiecture 
     */
    void loadStateFromFile(std::string filename);

    /**
     * This states whether or not these two *architectures* are equivalent.
     * Nothing is determined by the state of the two networks. 
     * To exaplain, two fully connected networks with the same number of layers
     *      and number of nodes in those layers are equivalent.
     * However, a fully connected network and a network that contains a convolutional
     *      layer are not equivalent
     * 
     * @return -> Whether the two architectures are equivalent
     */
    bool isEqualArchitecture(LayeredNetwork* otherNetwork);

    /**
     *  Given another network, copy its state into this network's state.
     *  If the two architecures are not equivalent (see isEqualArchitecture) then nothing will happen
     */
    void copyState(LayeredNetwork* otherNetwork);

    /**
     * Given a particular Database, this will train the network for one epoch.
     * This will train with the specified functions from the network's construction 
     * And the epoch size is determined by the database itself
     * 
     * This will also automatically update the network as its gradients are calculated, and handles batching if neccessary
     */
    void trainEpoch(Database* database);

    /**
     * Given a database, this will evaluate the state of the network. This is done by calculating
     *      the error of the network over the database's test data.
     * This error is the sum of (output - label)^2 for all of the output nodes over the test data
     */
    double evaluate(Database* database);

    /**
     * Takes in the given data, and sets it as the pointer for the input matricies
     * Then this data is propagated as expected through the network.
     * No gradients are calculated here. However, if gradient calculations are desired, then the calculation must happen
     * just after this function. This is because the gradient is dependent on the input to the network.
     * 
     * @return -> The output of the network
     */
    Matrix* feedForward(double* input);

    /**
     *  Given some labels of correct answers, the network will calculate the lost term (output - label) and then 
     *      will go to each layer (starting from the last going to beginning) and have it calculate the error with respect to its weights.
     *  This will not preform any updates to the weights themselves, rather it will simply calculate the gradient with respect to the parameters
     *      and add it to a running sum (for batch gradient).
     *  
     * @param labels -> A pointer to the data labels, the length of which must be equivalent to inputNRows * inputNCols * inputMatrixCount
     */
    void calculateSuprivisedGradients(double* labels);

    /**
     *  Given an output node (index) update the gradient all the way through the network, given the loss value...
     *  For suprivised learning, this is simply output - label, however this function proves useful when using 
     *  the Bellman equation. That way, the loss function can be calculated for the one Q-value output node
     *  and the gradient is calculated all the way down the network, just with the loss of one node.
     * 
     *  This gradient is calculated and added to an internal gradient matrix. Call "update" in order to make 
     *      use of the gradients
     * 
     * @param loss -> The loss value for the output node specified
     * @param index -> The index of the output node
     */
    void addGradient(double loss, unsigned int index);

    /**
     * Returns an object that describes basic information about the network
     * 
     * @return -> The object that describes basic information about the network
     */
    NetworkInformation& getNetworkInformation();

    /**
     * @return -> The output matrix of this network
     */
    Matrix* getOutput();

    /**
     * With whatever learning function specified in this network's construction, update the parameters with the 
     *      gradients internalized over a series of feedforward and gradient calculations
     */
    void update();

    std::vector<std::unique_ptr<Layer>>& getLayers();

    int getInputMatrixCount();
    int getInputNRows();
    int getInputNCols();

    int getOutputMatrixCount();
    int getOutputNRows();
    int getOutputNCols();
};
#endif