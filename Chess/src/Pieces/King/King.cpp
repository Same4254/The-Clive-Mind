#include "Pieces/King/King.hpp"

King::King(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

King::~King() {
    
}

bool King::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}