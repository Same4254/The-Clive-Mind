#include "Pieces/Piece.hpp"

#include "Board.hpp"

Piece::Piece(TEAM team, char displayCharacter) : team(team), displayCharacter(displayCharacter) {

}

Piece::~Piece() {

}

void Piece::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    // pieces2D[7][8] -= PieceFunctionality::getInstance().pieceValues[pieces2D[endRow][endColumn]];

    pieces2D[endRow][endColumn] = pieces2D[startRow][startColumn];
    pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
}

Piece::TEAM Piece::getTeam() { return team; }
char Piece::getDisplayCharacter() { return displayCharacter; }