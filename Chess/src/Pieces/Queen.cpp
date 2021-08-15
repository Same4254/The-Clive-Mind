#include "Pieces/Queen.hpp"

Queen::Queen(const PieceFunctionalityIndex piece, const char renderCharacter) : PieceFunctionality(piece, renderCharacter, true), bishop(piece, renderCharacter), rook(piece, renderCharacter) {

}

void Queen::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {
    bishop.fillMoves(boardState, movesToFill, amtMovesFilled, boardRow, boardCol, boardPositionIndex);
    rook.fillMoves(boardState, movesToFill, amtMovesFilled, boardRow, boardCol, boardPositionIndex);
}