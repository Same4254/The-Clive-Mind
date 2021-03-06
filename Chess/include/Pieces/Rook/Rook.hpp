#ifndef ROOK_HPP
#define ROOK_HPP

#include "Pieces/Piece.hpp"

class Rook : public Piece {
public:
    Rook(TEAM team, char displayCharacter);
    ~Rook();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual bool canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);
};
#endif