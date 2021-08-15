#pragma once

#include <array>

#include "Types.hpp"

/**
 * Represents the general state of the board.
 *  - Pieces on the board
 *  - Hash of the board
 *  - The turn
 * 
 * This is meant to be a POD class to allow quick copying
 */
class BoardState {
public:
    static const BoardPieceIndex nRows = 8;
    static const BoardPieceIndex nCols = 8;

    //1D allocation of pieces for 1D access
    std::array<PieceFunctionalityIndex, nRows * nCols> pieces;

    //Cheeky trick to allow 2D access while having just 1D allocation. Each element is a pointer to a row in the board
    std::array<PieceFunctionalityIndex*, nRows> pieces2D;

    BoardHash hash;
    bool player1Turn;

    /**
     *  Given the 1D allocation of pieces, set the 2D row pointers to the elements in the 1D allocation
     *  Call this after the board state is copied or created
     * 
     *  Not using a copy constructor so that this stays as a POD class. It's unfortunate, but don't forget this....
     */
    void set2DPointers();
};