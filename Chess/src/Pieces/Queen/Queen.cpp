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

bool Queen::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType temp = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = temp;

        return true;
    }
    
    return false;
}