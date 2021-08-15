#include "Pieces/PawnDouble.hpp"

PawnDouble::PawnDouble(const PieceFunctionalityIndex piece, const char renderCharacter) : Pawn(piece, renderCharacter) {
    if(teamModularity == TEAM::BLACK)
        moveOffsets.push_back(std::make_pair(2, 0));
    else
        moveOffsets.push_back(std::make_pair(-2, 0));
}

void PawnDouble::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {
    Pawn::fillMoves(boardState, movesToFill, amtMovesFilled, boardRow, boardCol, boardPositionIndex);

    auto &p1 = moveOffsets.at(2);
    auto &p2 = moveOffsets.at(3);

    if(boardState.pieces2D[boardRow + p1.first][boardCol + p1.second] == PIECES::EMPTY && boardState.pieces2D[boardRow + p2.first][boardCol + p2.second] == PIECES::EMPTY) {
        takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, teamModularity == TEAM::BLACK ? PIECES::BLACK_PAWN : PIECES::WHITE_PAWN, boardRow, boardCol, boardRow + p2.first, boardCol + p2.second);
    }
}