#include "Pieces/Knight.hpp"

Knight::Knight(const PieceFunctionalityIndex piece, const char renderCharacter) : PieceFunctionality(piece, renderCharacter, true) {
    moveOffsets.push_back(std::make_pair(-2, 1));
    moveOffsets.push_back(std::make_pair(-1, 2));

    moveOffsets.push_back(std::make_pair(1, 2));
    moveOffsets.push_back(std::make_pair(2, 1));

    moveOffsets.push_back(std::make_pair(2, -1));
    moveOffsets.push_back(std::make_pair(1, -2));

    moveOffsets.push_back(std::make_pair(-1, -2));
    moveOffsets.push_back(std::make_pair(-2, -1));
}

void Knight::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {
    for(auto &p : moveOffsets) {
        int newRow = boardRow + p.first;
        int newCol = boardCol + p.second;

        if(newRow >= 0 && newRow < BoardState::nRows && newCol >= 0 && newCol < BoardState::nCols) 
            takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, piece, boardRow, boardCol, newRow, newCol);
    }
}