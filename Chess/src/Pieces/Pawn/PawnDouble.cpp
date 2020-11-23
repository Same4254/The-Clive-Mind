#include "Pieces/Pawn/PawnDouble.hpp"

PawnDouble::PawnDouble(TEAM team, char displayCharacter) : Pawn(team, displayCharacter) {

}

PawnDouble::~PawnDouble() {
    
}

bool PawnDouble::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return false;
}