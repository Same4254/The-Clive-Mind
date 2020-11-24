#include "Pieces/Bishop/Bishop.hpp"
#include "Pieces/PieceFunctionality.hpp"

Bishop::Bishop(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {

}

Bishop::~Bishop() {
    
}

bool Bishop::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    //Check if the end coordinate is on the diagonal
    if(abs(startRow - endRow) != abs(startColumn - endColumn))
        return false;

    //Check if the piece on the end coordinate is on the same team
    if(pieces2D[endRow][endColumn] != EMPTY && (pieces2D[startRow][startColumn] % 2) == (pieces2D[endRow][endColumn] % 2))
        return false;

    int rowFactor = endRow > startRow ? 1 : -1;
    int columnFactor = endColumn > startColumn ? 1 : -1;

    int tempRow = startRow + rowFactor;
    int tempCol = startColumn + columnFactor;

    while(tempRow >= 0 && tempRow <= 7 && tempCol >= 0 && tempCol <= 7) {
        if(tempCol == endColumn && tempRow == endRow)
            return true;

        if(pieces2D[tempRow][tempCol] != EMPTY)
            return false;

        tempRow += rowFactor;
        tempCol += columnFactor;
    }

    return false;
}

bool Bishop::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType temp = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = temp;

        return true;
    }
    
    return false;
}