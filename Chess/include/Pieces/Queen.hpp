#pragma once

#include "Pieces/PieceFunctionality.hpp"
#include "Pieces/BoardMovePieces/Bishop.hpp"
#include "Pieces/BoardMovePieces/Rook.hpp"

class Queen : public PieceFunctionality {
private:
    //What is a queen? Really she is just a bishop and a rook
    Bishop bishop;
    Rook rook;

public:
    Queen(const PieceFunctionalityIndex piece, const char renderCharacter);

    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const;
};