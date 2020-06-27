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

    void calculateGradients();

public:
    LayeredNetwork(int inputMatrixCount, int inputNRows, int inputNCols);
    ~LayeredNetwork();

    void initialize();
    void writeStructureToFile(std::string filename);
    void writeStateToFile(std::string filename);
    void loadStateFromFile(std::string filename);

    void trainEpoch(Database* database);
    double evaluate(Database* database);

    Matrix* feedForward(double* input);
    void calculateGradients(double* labels);

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