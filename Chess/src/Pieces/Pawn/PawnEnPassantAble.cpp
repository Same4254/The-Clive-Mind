#include "Pieces/Pawn/PawnEnPassantAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

PawnEnPassantAble::PawnEnPassantAble(TEAM team, char displayCharacter) : Pawn(team, displayCharacter) {

}

PawnEnPassantAble::~PawnEnPassantAble() {
    
}

bool PawnEnPassantAble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return Pawn::canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}