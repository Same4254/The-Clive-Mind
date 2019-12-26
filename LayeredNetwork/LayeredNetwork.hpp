// #include "ConvolutionalLayer.hpp"
#include "ActivationLayer.hpp"
// #include "PoolingLayer.hpp"
#include "FullyConnectedLayer.hpp"
// #include "FlatteningLayer.hpp"

#ifndef NETWORK_INFORMATION_CPP
#define NETWORK_INFORMATION_CPP

#include "NetworkInformation.cpp"

#endif

class LayeredNetwork {
    private:
        NetworkInformation* networkInformation;
        Layer** layers;
        int amountOfLayers;

        int inputMatrixCount;
        int inputNRows;
        int inputNCols;

        int outputMatrixCount;
        int outputNRows;
        int outputNCols;

        Matrix* error;

    public:
        LayeredNetwork(int amountOfLayers, int inputMatrixCount, int inputNRows, int inputNCols, int outputMatrixCount, int outputNRows, int outputNCols) { 
            this->amountOfLayers = amountOfLayers;

            this->inputMatrixCount = inputMatrixCount;
            this->inputNRows = inputNRows;
            this->inputNCols = inputNCols;

            this->outputMatrixCount = outputMatrixCount;
            this->outputNRows = outputNRows;
            this->outputNCols = outputNCols;

            layers = new Layer*[amountOfLayers];

            error = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
            for(int i = 0; i < outputMatrixCount; i++)
                new (&error[i]) Matrix(outputNRows, outputNCols);

            networkInformation = new NetworkInformation();
        }

        ~LayeredNetwork() {
            for(int i = 0; i < amountOfLayers; i++)
                delete layers[i];
            delete[] layers;

            for(int i = 0; i < outputMatrixCount; i++)
                delete &error[i];

            delete networkInformation;
        }

        void initialize() { 
            layers[0]->setInputMatrixCount(inputMatrixCount);
            layers[0]->setInputNRows(inputNRows);
            layers[0]->setInputNCols(inputNCols);

            layers[amountOfLayers - 1]->setOutputMatrixCount(outputMatrixCount);
            layers[amountOfLayers - 1]->setOutputNRows(outputNRows);
            layers[amountOfLayers - 1]->setOutputNCols(outputNCols);

            for(int i = 0; i < amountOfLayers; i++) {
                layers[i]->Layer::initialize();
                layers[i]->initialize();
            }
        }

        Matrix* feedForward(Matrix* input) {
            for(int i = 0; i < amountOfLayers; i++) {
                if(i == 0) 
                    layers[0]->feedForward(input);
                else
                    layers[i]->feedForward(layers[i - 1]->getOutput());
            }

            return layers[amountOfLayers - 1]->getOutput();
        }

        void calculateGradients(Matrix* labels) {
            for(int i = 0; i < outputMatrixCount; i++)
                getOutput()->subtract((&labels[i]), &error[i]);

            for(int i = amountOfLayers - 1; i >= 0; i--) {
                if(i == amountOfLayers - 1)
                    layers[i]->calculateGradient(error);
                else 
                    layers[i]->calculateGradient(layers[i + 1]->getGradient());
            }
        }

        void update() {
            for(int i = 0; i < amountOfLayers; i++)
                layers[i]->update();
        }

        NetworkInformation* getNetworkInformation() { return networkInformation; }
        Matrix* getOutput() { return layers[amountOfLayers - 1]->getOutput(); }

        int getAmountOfLayers() { return amountOfLayers; }
        Layer** getLayers() { return layers; }
};