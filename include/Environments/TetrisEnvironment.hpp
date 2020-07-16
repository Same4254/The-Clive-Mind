#ifndef TETRIS_ENVIRONMENT_H
#define TETRIS_ENVIRONMENT_H

#include "Matrix.hpp"

class TetrisEnvironment {
private:
    Matrix* boardMatrix;
    unsigned int stateLength;

public:
    TetrisEnvironment(unsigned int stateLength);
    virtual ~TetrisEnvironment();

    virtual bool isLegalAction(double* state, unsigned int actionIndex);
    virtual double preformAction(double* originalState, double* nextState, unsigned int actionIndex);
};
#endif