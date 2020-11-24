#ifndef PIECE_FUNCTIONALITY_HPP
#define PIECE_FUNCTIONALITY_HPP

#include <iostream>
#include <vector>

#include "Pieces/Piece.hpp"

class Board;

class PieceFunctionality {
private:
    Piece **pieces;

    PieceFunctionality();
    ~PieceFunctionality();

public:
    static PieceFunctionality& getInstance() {
        static PieceFunctionality instance;
        return instance;
    }

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
     * 
     * @return -> whether the piece was moved
     */
    bool move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

    void generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn);

    char getDisplayCharacter(PieceIndexType pieceIndex);
};

#endif