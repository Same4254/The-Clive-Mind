#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

class ActivationLayer: public Layer {
    private: 
        ActivationFunction* function;

    public:
        ActivationLayer(ActivationFunction* function) { 
            this->function = function;
        }

        void initialize() { 
            
        }

        Matrix* feedForward(Matrix* input) { 

        }

        Matrix* backpropogate(Matrix* error) { 
            
        }
};