#ifndef PAWN_DOUBLE_HPP
#define PAWN_DOUBLE_HPP

#include "Pieces/Piece.hpp"
#include "Pieces/Pawn/Pawn.hpp"

class PawnDouble : public Pawn {
public:
    PawnDouble(TEAM team, char displayCharacter);
    virtual ~PawnDouble();

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