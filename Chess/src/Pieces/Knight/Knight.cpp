#include "Pieces/Knight/Knight.hpp"
#include "Pieces/PieceFunctionality.hpp"

Knight::Knight(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Knight::~Knight() {
    
}

bool Knight::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    int rowDifference = abs(startRow - endRow);
    int colDifference = abs(startColumn - endColumn);

    if(rowDifference != colDifference && (rowDifference == 1 || rowDifference == 2) && (colDifference == 1 || colDifference == 2))
        return true;

    return false;
}