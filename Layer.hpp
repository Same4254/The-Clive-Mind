#include "./Matrix/Matrix.cpp"
#include "ActivationFunction.hpp"
#include "SigmoidFunction.hpp"

class Layer {
    public:
        virtual void initialize() = 0;
        virtual Matrix* feedForward(Matrix* input);
        virtual Matrix* backpropogate(Matrix* error);
};