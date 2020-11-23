#include "Pieces/Bishop/Bishop.hpp"

Bishop::Bishop(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Bishop::~Bishop() {
    
}

bool Bishop::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}