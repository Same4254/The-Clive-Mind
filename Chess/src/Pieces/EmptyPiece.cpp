#include "Pieces/EmptyPiece.hpp"

EmptyPiece::EmptyPiece(char displayCharacter) : Piece(Piece::TEAM::NONE, displayCharacter) {

}

EmptyPiece::~EmptyPiece() {
    
}

bool EmptyPiece::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}