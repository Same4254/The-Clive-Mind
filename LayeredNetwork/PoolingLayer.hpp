#ifndef LAYER_HPP
#define LAYER_HPP

#include "Layer.hpp"

#endif

#include <vector>

class PoolingLayer: public Layer {
    private:
        Matrix* poolIndecies;

        int size;
        int numPoolIndecies;

    public:
        PoolingLayer(Layer** networkLayers, int index, int size) : Layer(networkLayers, index) {
            this->size = size;
        }

        void initialize() {
            if(inputNRows % size != 0)
                throw std::invalid_argument("Invalid size for pooling layer");

            output = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            gradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
            poolIndecies = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

            numPoolIndecies = (inputNRows / size) * (inputNCols / size);

            for(int i = 0; i < inputMatrixCount; i++) {
                new (&output[i]) Matrix(inputNRows / size, inputNCols / size);
                new (&gradient[i]) Matrix(inputNRows, inputNCols);
                new (&poolIndecies[i]) Matrix(numPoolIndecies, 2);
            }

            outputMatrixCount = inputMatrixCount;
            outputNRows = inputNRows / size;
            outputNCols = inputNCols / size;
        }

        Matrix* feedForward(Matrix* input) {
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

        Matrix* backpropogate(Matrix* error) {
            for(int i = 0; i < outputMatrixCount; i++) {
                (&gradient[i])->clear();// could look at the previous positions to set them only
                
                for(int index = 0; index < numPoolIndecies; index++) {
                    (&gradient[i])->set((&poolIndecies[i])->at(index, 0), (&poolIndecies[i])->at(index, 1), (&error[i])->getData()[index]);
                }
            }

            return gradient;
        }
};