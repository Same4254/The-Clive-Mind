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
    static const int STATE_LENGTH;
    static const int SCORE_INDEX;

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

    /**
     * A variable to store how many layers deep this board is in minimax. 
     * This is used to break ties between possible moves and ensure the quickest route to the highest score is taken
     * This may be moved into the board state pointer rather than the Board class
     */ 
    int minimaxDepth;

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
     * Clears the board of all pieces, and sets the score to 0
     */
    void clear();

    /**
     * This will take the current state of the board and generate all of the possible moves, for the given color, in the form of new boards.
     * These new boards will be push_back-ed onto the possibleMoves vector.
     * 
     * @param possibleMoves -> The vector to add all of the possible moves to, in the form of the resulting board from performing the move
     * @param team -> The team to generate the moves for
     */
    void generateAllMoves(std::vector<Board> &possibleMoves, Piece::TEAM team);

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