#include "Layer.hpp"

class LayeredNetwork {
    public:
        Layer** layers;
        int amountOfLayers;

    public:
        LayeredNetwork(int amountOfLayers) { 
            this->amountOfLayers = amountOfLayers;
        }

        void initialize() { 
            for(int i = 0; i < amountOfLayers; i++) {
                layers[i]->initialize();
            }
        }
};

int main() { 
    LayeredNetwork network(1);

    
}