#include "Pieces/Rook/Rook.hpp"

Rook::Rook(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Rook::~Rook() {
    
}

bool Rook::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}