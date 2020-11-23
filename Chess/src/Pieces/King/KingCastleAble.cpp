#include "Pieces/King/KingCastleAble.hpp"

KingCastleAble::KingCastleAble(TEAM team, char displayCharacter) : King(team, displayCharacter) {

}

KingCastleAble::~KingCastleAble() {
    
}

bool KingCastleAble::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}