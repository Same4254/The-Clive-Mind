#include "Pieces/Pawn/PawnEnPassantAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

PawnEnPassantAble::PawnEnPassantAble(TEAM team, char displayCharacter) : Pawn(team, displayCharacter) {

}

PawnEnPassantAble::~PawnEnPassantAble() {
    
}

bool PawnEnPassantAble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return Pawn::canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}

bool PawnEnPassantAble::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_PAWN : Piece::TYPE::WHITE_PAWN);

        return true;
    }
    
    return false;
}