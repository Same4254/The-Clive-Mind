#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

class PoolingLayer: public Layer {
    private:
        Matrix* output;
        Matrix* gradient;

        int size;
        int*** poolIndecies;
        int numPoolIndecies;

    public:
        PoolingLayer(int size) {
            this->size = size;
        }

        void initialize() {
            if(inputNRows % size != 0)
                throw std::invalid_argument("Invalid size for pooling layer");

            output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

        }

        Matrix* feedForward(Matrix* input) {
            for(int i = 0; i < inputMatrixCount; i++) {

                int index = 0;
                for(int row = 0; row <= inputNRows; row += size) {
                for(int col = 0; col <= inputNCols; col += size) {
                    int positionRow = 0;
                    int positionCol = 0;

                    for(int r = row; r < row + size; r++) {
                    for(int c = col; c < col + size; c++) {
                        if((&input[i])->at(r, c) > (&input[i])->at(row, col)){
                            positionRow = r;
                            positionCol = c;
                        }
                    }}

                    (&output[i])->set(row / size, col / size, (&input[i])->at(positionRow, positionCol));

                    poolIndecies[i][index][0] = positionRow;
                    poolIndecies[i][index][1] = positionCol;
                    index++;
                }}
            }

            return output;
        }

        Matrix* backpropogate(Matrix* error) {
            for(int i = 0; i < outputMatrixCount; i++) {
                (&gradient[i])->clear();// could look at the previous positions to set them only
                
                for(int index = 0; index < numPoolIndecies; index++) {
                    (&gradient[i])->set(poolIndecies[i][index][0], poolIndecies[i][index][1], (&error[i])->getData()[index]);
                }
            }

            return gradient;
        }
};