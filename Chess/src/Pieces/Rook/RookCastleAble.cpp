#include "Pieces/Rook/RookCastleAble.hpp"

RookCastleAble::RookCastleAble(TEAM team, char displayCharacter) : Rook(team, displayCharacter) {

}

RookCastleAble::~RookCastleAble() {
    
}

bool RookCastleAble::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}