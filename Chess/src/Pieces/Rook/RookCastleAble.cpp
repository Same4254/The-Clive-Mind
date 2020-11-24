#include "Pieces/Rook/RookCastleAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

RookCastleAble::RookCastleAble(TEAM team, char displayCharacter) : Rook(team, displayCharacter) {

}

RookCastleAble::~RookCastleAble() {
    
}

bool RookCastleAble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return Rook::canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}