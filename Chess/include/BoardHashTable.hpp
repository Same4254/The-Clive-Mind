#pragma once

#include <random>
#include <iostream>

#include "Types.hpp"

class BoardHashTable {
private:
    std::mt19937_64 random;
    std::uniform_int_distribution<BoardHash> uniformDistribution;

    //2D and 1D access to the hashes (the hashes are the same in the two representations, just different means of access)
    std::vector<std::vector<std::vector<BoardHash>>> pieceLocationToHash;
    std::vector<std::vector<BoardHash>> pieceLocationIndexToHash;

public:
    /**
     * Given the shape of the board and the number of types of pieces on the board,
     *      initialize the hashes for every type of piece at every location
     */
    BoardHashTable(uint32_t numPieceTypes, uint32_t numRows, uint32_t numCols);

    inline BoardHash getHash(const PieceFunctionalityIndex pieceIndex, const BoardPieceIndex row, const BoardPieceIndex col) const {
        return pieceLocationToHash[pieceIndex][row][col];
    }

    inline BoardHash getHash(const PieceFunctionalityIndex pieceIndex, const BoardPieceIndex index) const {
        return pieceLocationIndexToHash[pieceIndex][index];
    }
};