#include "Pieces/Piece.hpp"

#include "Board.hpp"

Piece::Piece(TEAM team, char displayCharacter) : team(team), displayCharacter(displayCharacter) {

}

Piece::~Piece() {

}

void Piece::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    pieces2D[endRow][endColumn] = pieces2D[startRow][startColumn];
    pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
}

void Piece::generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn) {
    for(size_t i = 0; i < rowMovementOffsets.size(); i++) {
        if(PieceFunctionality::getInstance().canMove(board.pieces2D, pieceRow, pieceColumn, pieceRow + rowMovementOffsets[i], pieceColumn + columnMovementOffsets[i])) {
            boards.push_back(Board(board));
            PieceFunctionality::getInstance().move(boards.at(boards.size() - 1).pieces2D, pieceRow, pieceColumn, pieceRow + rowMovementOffsets[i], pieceColumn + columnMovementOffsets[i]);
        }
    }
}