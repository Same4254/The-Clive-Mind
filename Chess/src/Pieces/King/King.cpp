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

bool King::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType temp = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = temp;

        return true;
    }
    
    return false;
}