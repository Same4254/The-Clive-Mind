#include "Pieces/Queen/Queen.hpp"
#include "Pieces/PieceFunctionality.hpp"

Queen::Queen(TEAM team, char displayCharacter) : Piece(team, displayCharacter), rookInstance(team, displayCharacter), bishopInstance(team, displayCharacter) {

}

Queen::~Queen() {
    
}

bool Queen::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    return rookInstance.canMove(pieces2D, startRow, startColumn, endRow, endColumn) ||
            bishopInstance.canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}