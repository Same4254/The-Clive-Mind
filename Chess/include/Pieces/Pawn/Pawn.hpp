#ifndef PAWN_HPP
#define PAWN_HPP

#include "Pieces/Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(TEAM team, char displayCharacter);
    virtual ~Pawn();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual bool canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn);
};
#endif