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
    void writeStructureToFile(std::string filename);
    void writeStateToFile(std::string filename);
    void loadStateFromFile(std::string filename);

    bool isEqualArchitecture(LayeredNetwork* otherNetwork);
    void copyState(LayeredNetwork* otherNetwork);

    void trainEpoch(Database* database);
    double evaluate(Database* database);

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
     * @param loss -> The loss value for the output node specified
     * @param index -> The index of the output node
     */
    void addGradient(double loss, unsigned int index);

    NetworkInformation& getNetworkInformation();
    Matrix* getOutput();
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