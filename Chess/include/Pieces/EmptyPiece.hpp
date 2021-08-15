#pragma once

#include "Pieces/PieceFunctionality.hpp"

class EmptyPiece : public PieceFunctionality {
public:
    EmptyPiece(const char renderCharacter);

    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const;
};