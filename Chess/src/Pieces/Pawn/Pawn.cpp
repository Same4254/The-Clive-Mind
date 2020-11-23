#include "Pieces/Pawn/Pawn.hpp"

Pawn::Pawn(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Pawn::~Pawn() {
    
}

bool Pawn::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}