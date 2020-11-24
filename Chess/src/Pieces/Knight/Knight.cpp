#include "Pieces/Knight/Knight.hpp"
#include "Pieces/PieceFunctionality.hpp"

Knight::Knight(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Knight::~Knight() {
    
}

bool Knight::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    int rowDifference = abs(startRow - endRow);
    int colDifference = abs(startColumn - endColumn);

    if(rowDifference != colDifference && (rowDifference == 1 || rowDifference == 2) && (colDifference == 1 || colDifference == 2) && 
            (pieces2D[endRow][endColumn] == Piece::TYPE::EMPTY || pieces2D[endRow][endColumn] % 2 != pieces2D[startRow][startColumn] % 2) )
        return true;

    return false;
}

bool Knight::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType temp = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = temp;

        return true;
    }
    
    return false;
}