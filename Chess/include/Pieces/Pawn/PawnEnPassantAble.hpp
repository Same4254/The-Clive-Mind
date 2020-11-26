#ifndef PAWN_EN_PASSANT_ABLE_HPP
#define PAWN_EN_PASSANT_ABLE_HPP

#include "Pieces/Piece.hpp"
#include "Pieces/Pawn/Pawn.hpp"

class PawnEnPassantAble : public Pawn {
public:
    PawnEnPassantAble(TEAM team, char displayCharacter);
    virtual ~PawnEnPassantAble();
};
#endif