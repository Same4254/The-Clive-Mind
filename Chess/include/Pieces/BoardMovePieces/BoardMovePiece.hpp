#pragma once

#include "Pieces/PieceFunctionality.hpp"

//This is the kind of piece that moves entirely along the board
class BoardMovePiece : public PieceFunctionality {
private:
    //What the piece turns into after its move
    //This is here for the castle-able rook to write that it turns into a rook
    PieceFunctionalityIndex newPiece;

public:
    BoardMovePiece(const PieceFunctionalityIndex piece, PieceFunctionalityIndex newPiece, const char renderCharacter);

    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const;
};