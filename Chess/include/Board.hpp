#ifndef BOARD_HPP
#define BOARD_HPP

#include <stdlib.h>
#include <string.h>

#include <iomanip>
#include <iostream>

#include "Pieces/PieceFunctionality.hpp"
#include "Pieces/Piece.hpp"

class Board {
public:
    //1D representation of the IDs of the pieces on the board
    PieceIndexType *pieces;

    //2D access to the board, same memory as the pieces pointer, just 2 dimensional
    PieceIndexType **pieces2D;

    Board();
    Board(const Board &other);
    ~Board();

    /**
     * Set the board to the starting piece layout
     */
    void setBoard();

    void clearBoard();

    void generateAllMoves(std::vector<Board> &possibleMoves, Piece::TEAM team);

    /**
     * Print the ID of each piece on the board
     */
    void printDebug();

    /**
     * Print the pieces on the board in human readable characters
     */
    void print();
};
#endif