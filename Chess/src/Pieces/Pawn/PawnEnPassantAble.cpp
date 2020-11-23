#include "Pieces/Pawn/PawnEnPassantAble.hpp"

PawnEnPassantAble::PawnEnPassantAble(TEAM team, char displayCharacter) : Pawn(team, displayCharacter) {

}

PawnEnPassantAble::~PawnEnPassantAble() {
    
}

bool PawnEnPassantAble::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}