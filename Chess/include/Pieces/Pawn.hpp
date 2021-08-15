#pragma once

#include "Pieces/PieceFunctionality.hpp"

class Pawn : public PieceFunctionality {
private:
    std::array<PIECES, 4> whitePromotionPieces, blackPromotionPieces;

public:
    Pawn(const PieceFunctionalityIndex piece, const char renderCharacter);

    void promote(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex nextBoardRow, const BoardPieceIndex nextBoardCol) const;

    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const;
};