#include "LayeredNetwork/Layers/PoolingLayer.hpp"

PoolingLayer::PoolingLayer(NetworkInformation& networkInformation, int index, int size) : Layer(networkInformation, Pool, index) { 
    this->size = size;
}

void PoolingLayer::initialize() {
    if(inputNRows % size != 0)
        throw std::invalid_argument("Invalid size for pooling layer");

    output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
    layerGradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
    poolIndecies = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

    numPoolIndecies = (inputNRows / size) * (inputNCols / size);

    for(int i = 0; i < inputMatrixCount; i++) {
        new (&output[i]) Matrix(inputNRows / size, inputNCols / size);
        new (&layerGradient[i]) Matrix(inputNRows, inputNCols);
        new (&poolIndecies[i]) Matrix(numPoolIndecies, 2);
    }

    outputMatrixCount = inputMatrixCount;
    outputNRows = inputNRows / size;
    outputNCols = inputNCols / size;
}

void PoolingLayer::toFile(FILE* file) {
    
}

void PoolingLayer::fromFile(FILE* file) {
    
}

Matrix* PoolingLayer::feedForward(Matrix* input) {
    for(int i = 0; i < inputMatrixCount; i++) {

        int index = 0;
        for(int row = 0; row <= inputNRows - size; row += size) {
        for(int col = 0; col <= inputNCols - size; col += size) {
            int positionRow = row;
            int positionCol = col;

            for(int r = row; r < row + size; r++) {
            for(int c = col; c < col + size; c++) {
                if((&input[i])->at(r, c) > (&input[i])->at(positionRow, positionCol)){
                    positionRow = r;
                    positionCol = c;
                }
            }}

            (&output[i])->set(row / size, col / size, (&input[i])->at(positionRow, positionCol));

            (&poolIndecies[i])->set(index, 0, positionRow);
            (&poolIndecies[i])->set(index, 1, positionCol);

            // poolIndecies[i][index][0] = positionRow;
            // poolIndecies[i][index][1] = positionCol;
            index++;
        }}
    }

    return output;
}

Matrix* PoolingLayer::backpropogate(Matrix* error) {
    for(int i = 0; i < outputMatrixCount; i++) {
        (&layerGradient[i])->clear();// could look at the previous positions to set them only
        
        for(int index = 0; index < numPoolIndecies; index++) {
            (&layerGradient[i])->set((&poolIndecies[i])->at(index, 0), (&poolIndecies[i])->at(index, 1), (&error[i])->getData()[index]);
        }
    }

    return layerGradient;
}