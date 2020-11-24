#ifndef BOARD_HPP
#define BOARD_HPP

#include <stdlib.h>

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
    ~Board();

    /**
     * Set the board to the starting piece layout
     */
    void setBoard();

    void clearBoard();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    bool canMove(int startRow, int startColumn, int endRow, int endColumn);

    /**
     * Given a location on the board, check if any piece can be moved onto that location
     */
    bool canBeMovedOnto(int row, int column);

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