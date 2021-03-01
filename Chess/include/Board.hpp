#ifndef BOARD_HPP
#define BOARD_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <random>

#include <unordered_map>
#include <unordered_set>

#include "Pieces/PieceFunctionality.hpp"
#include "Pieces/Piece.hpp"

typedef unsigned long long hash_type;

class Board {
private:
    int countMovesHelper(std::unordered_set<hash_type> &visitedPositions, bool whiteToMove, int plys);

public:
    static const int STATE_LENGTH;

    hash_type hash;

    /**
     * 1D representation of the IDs of the pieces on the board
     * 
     * The layout of this memory is that the first 64 elements are the places on the board. 
     * The last element is the score, where a negative number means that black is up pieces, and positive means that white is up pieces
     * How much "down" and "up" depends on the pieces. The point value of any piece is subtracted when they are captured. See include/Pieces/PieceFunctionality.cpp for the score of each piece
     */
    PieceIndexType *state;

    //2D access to the board, same memory as the pieces pointer, just 2 dimensional. 
    //This offers the ability to access by rows and columns while having the actual memory as a plain 1D pointer
    PieceIndexType **pieces2D;

    std::array<std::array<std::array<hash_type, 21>, 8>, 8> pieceToHash;

    /**
     * This initializes the memory of the board state. It is initially a clear board of "empty pieces" 
     */
    Board();

    /**
     * A copy constructor. This will copy the state of the other board entirely
     */
    Board(const Board &other);

    /**
     * Be freeeeee!!!
     */
    ~Board();

    /**
     * Set the board to the starting piece layout of a chess game
     */
    void setBoard();

    /**
     *  Generates a hash for the current position of the pieces
     */
    hash_type generateHash();

    /**
     *  Count Moves from white's perspective
     */
    int countMoves(bool whiteToMove, int plys);

    /**
     * Clears the board of all pieces, and sets the score to 0
     */
    void clear();

    /**
     * Print the numerical ID of each piece on the board. This ID represent the number in the Enumaration in the Piece class.
     */
    void printDebug();

    /**
     * Print the pieces on the board in human readable characters
     */
    void print();
};
#endif