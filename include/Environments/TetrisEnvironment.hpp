#ifndef TETRIS_ENVIRONMENT_H
#define TETRIS_ENVIRONMENT_H

#include "Matrix.hpp"

class TetrisEnvironment {
private:
    Matrix* boardMatrix;

public:
    TetrisEnvironment();
    ~TetrisEnvironment();

    void initialState(double* stateToFill);
    bool isLegalAction(double* state, unsigned int actionIndex);
    double preformAction(double* originalState, double* nextState, unsigned int actionIndex);
};
#endif