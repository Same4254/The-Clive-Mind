#include "Pieces/King/King.hpp"

King::King(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

King::~King() {
    
}

bool King::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(abs(startRow - endRow) > 1 || abs(startColumn - endColumn) > 1)
        return false;

    if(pieces2D[endRow][endColumn] == EMPTY || pieces2D[startRow][startColumn] % 2 != pieces2D[endRow][endColumn] % 2)
        return true;

    return false;
}