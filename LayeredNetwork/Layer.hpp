#include "../ActivationFunction.hpp"

class Layer {
    protected:
        int inputMatrixCount;
        int inputNRows;
        int inputNCols;

        int outputMatrixCount;
        int outputNRows;
        int outputNCols;

    public:
        virtual void initialize() = 0;
        virtual Matrix* feedForward(Matrix* input) = 0;
        virtual Matrix* backpropogate(Matrix* error) = 0;
};