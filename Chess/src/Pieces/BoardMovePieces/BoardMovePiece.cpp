#include "Pieces/BoardMovePieces/BoardMovePiece.hpp"

BoardMovePiece::BoardMovePiece(const PieceFunctionalityIndex piece, PieceFunctionalityIndex newPiece, const char renderCharacter) : PieceFunctionality(piece, renderCharacter, true), newPiece(newPiece) {

} 

void BoardMovePiece::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {
    int newRow = 0, newCol = 0;
    
    for(int i = 0; i < moveOffsets.size(); i += BoardState::nRows - 1) {
        for(int j = i; j < i + BoardState::nRows - 1; j++) {
            newRow = boardRow + moveOffsets.at(j).first;
            newCol = boardCol + moveOffsets.at(j).second;

            if(newRow < 0 || newRow >= BoardState::nRows || newCol < 0 || newCol >= BoardState::nCols)
                break;

            //Can't take your own pieces
            if(boardState.pieces2D[newRow][newCol] != PIECES::EMPTY && boardState.pieces2D[newRow][newCol] % 2 == teamModularity)
                break;

            takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, newPiece, boardRow, boardCol, newRow, newCol);

            //Can't jump over an enemy piece
            if(boardState.pieces2D[newRow][newCol] != PIECES::EMPTY && boardState.pieces2D[newRow][newCol] % 2 != teamModularity)
                break;
        }
    }
}