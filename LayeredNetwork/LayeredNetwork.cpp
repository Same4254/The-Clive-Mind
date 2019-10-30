#include "ConvolutionalLayer.cpp"
#include "ActivationLayer.hpp"

class LayeredNetwork {
    public:
        Layer** layers;
        int amountOfLayers;

    public:
        LayeredNetwork(int amountOfLayers) { 
            this->amountOfLayers = amountOfLayers;
        }

        void initialize() { 
        
        }
};

int main() { 
    LayeredNetwork network(1);

    
}