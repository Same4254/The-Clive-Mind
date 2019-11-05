#include "ConvolutionalLayer.cpp"
#include "ActivationLayer.hpp"
#include "PoolingLayer.hpp"

class LayeredNetwork {
    public:
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
            for(int i = 0; i < outputMatrixCount; i++) {
                new (&error[i]) Matrix(outputNRows, outputNCols);
            }
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

        void backpropogate(Matrix* labels) {
            for(int i = 0; i < outputMatrixCount; i++) {
                getOutput()->subtract((&labels[i]), &error[i]);
            }

            for(int i = amountOfLayers - 1; i >= 0; i--) {
                if(i == amountOfLayers - 1)
                    layers[i]->backpropogate(error);
                else 
                    layers[i]->backpropogate(layers[i + 1]->getGradient());
            }
        }

        Matrix* getOutput() { return layers[amountOfLayers - 1]->getOutput(); }
        Layer** getLayers() { return layers; }
};

int main() { 
    ActivationFunction* sigmoid = new SigmoidFunction();

    LayeredNetwork network(2, 1, 4, 4, 1, 2, 2);
    PoolingLayer pool(network.layers, 0, 2);
    ActivationLayer activation(network.layers, 1, sigmoid);

    network.layers[0] = &pool;
    network.layers[1] = &activation;

    network.initialize();

    Matrix m(4, 4, -1, 1);
    printf("Input: \n");

    m.print();

    network.feedForward(&m);

    printf("\n Output: \n");
    network.getOutput()->print();

    Matrix label(2, 2, -1, 1);
    network.backpropogate(&label);
}