#ifndef PIECE_FUNCTIONALITY_HPP
#define PIECE_FUNCTIONALITY_HPP

#include <iostream>
#include <vector>

#include "Pieces/Piece.hpp"

/**
 * The idea of this class is to separate the behavior of the pieces on the board from the board itself.
 * The goal was to not generate functional objects, each object representing a peice on the board, *for every board*
 * This is because boards are going to be constructed FREQUENTLY. Limited overhead is preffered.
 * 
 * The singleton design was neccessary to initialize the pointers to the functional objects that represent the piece behaviors
 * Notice that in this design the behavior is separate from the board itself. The desire was to avoid a massive file of functions, and allow peices to inherit from one another
 * This is useful for the different Pawn and King states
 * 
 * The use for this class is to access the static instance of the class, and then provide the board and position for a piece to do something.
 * The number in the board, at that position, corresponds to the index in the pieceFunctions pointer. 
 * So if we want to move a piece on the board, we give the instance of pieceFunctionality the boardState, and the position. 
 *      It will look up the number in that position and get the functional object to do whatever is desired
 */
class PieceFunctionality {
private:
    /**
     * Pointer to pointers becuase object pointers with inheritence in C++ is interesting
     * These are objects (more like state-less utility functions with some inheritence), that perform piece behavior
     */
    Piece **pieceFunctions;

    PieceFunctionality();
    ~PieceFunctionality();

public:
    /**
     *  An array that stores the piece value of any given piece
     */
    PieceIndexType pieceValues[21];

    static PieceFunctionality& getInstance() {
        static PieceFunctionality instance;
        return instance;
    }

    bool isKingInCheck(PieceIndexType **pieces2D, Piece::TEAM team);

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    bool canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

    bool canAnyPieceMoveTo(PieceIndexType **pieces2D, int row, int column, Piece::TEAM team);
    bool canAnyWhitePieceMoveTo(PieceIndexType **pieces2D, int row, int column);
    bool canAnyBlackPieceMoveTo(PieceIndexType **pieces2D, int row, int column);

    /**
     * Given a piece on the board, move that piece to the target coordinate
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    void move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

    void generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn);

    char getDisplayCharacter(PieceIndexType pieceIndex);
};

#endif