#include "Pieces/Rook/RookCastleAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

RookCastleAble::RookCastleAble(TEAM team, char displayCharacter) : Rook(team, displayCharacter) {

}

RookCastleAble::~RookCastleAble() {
    
}

bool RookCastleAble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return Rook::canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}

void RookCastleAble::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    // if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        pieces2D[7][8] -= PieceFunctionality::getInstance().pieceValues[pieces2D[endRow][endColumn]];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_ROOK : Piece::TYPE::WHITE_ROOK);

    //     return true;
    // }
    
    // return false;
}