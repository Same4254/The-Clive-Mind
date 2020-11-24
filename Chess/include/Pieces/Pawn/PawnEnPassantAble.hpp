#ifndef PAWN_EN_PASSANT_ABLE_HPP
#define PAWN_EN_PASSANT_ABLE_HPP

#include "Pieces/Piece.hpp"
#include "Pieces/Pawn/Pawn.hpp"

class PawnEnPassantAble : public Pawn {
public:
    PawnEnPassantAble(TEAM team, char displayCharacter);
    virtual ~PawnEnPassantAble();

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