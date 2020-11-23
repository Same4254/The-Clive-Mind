#include "Pieces/Queen/Queen.hpp"

Queen::Queen(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Queen::~Queen() {
    
}

bool Queen::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}