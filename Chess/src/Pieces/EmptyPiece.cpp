#include "Pieces/EmptyPiece.hpp"

EmptyPiece::EmptyPiece(const char renderCharacter) : PieceFunctionality(PIECES::EMPTY, renderCharacter, false) {

}

void EmptyPiece::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {

}