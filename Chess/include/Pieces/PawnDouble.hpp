#pragma once

#include "Pieces/Pawn.hpp"

class PawnDouble : public Pawn {
public:
    PawnDouble(const PieceFunctionalityIndex piece, const char renderCharacter);

    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const;
};