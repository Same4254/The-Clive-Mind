#include "Pieces/EmptyPiece.hpp"
#include "Pieces/PieceFunctionality.hpp"

EmptyPiece::EmptyPiece(char displayCharacter) : Piece(Piece::TEAM::NONE, displayCharacter) {

}

EmptyPiece::~EmptyPiece() {
    
}

bool EmptyPiece::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return false;
}

bool EmptyPiece::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return false;
}