#include "Layer.hpp"

class ActivationLayer :public Layer {
    private: 
        ActivationFunction* function;

    public:
        ActivationLayer(ActivationFunction* function) { 
            // this->function = function;
        }

        void initialize() { 
            
        }

        Matrix* feedForward(Matrix* input) { 

        }

        Matrix* backpropogate(Matrix* error) { 
            
        }
};