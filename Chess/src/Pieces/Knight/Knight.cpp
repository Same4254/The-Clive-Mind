#include "Pieces/Knight/Knight.hpp"

Knight::Knight(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Knight::~Knight() {
    
}

bool Knight::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}