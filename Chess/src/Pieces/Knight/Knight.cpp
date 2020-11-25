#include "Pieces/Knight/Knight.hpp"
#include "Pieces/PieceFunctionality.hpp"

#include "Board.hpp"

Knight::Knight(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {
    rowMovementOffsets.push_back(2);
    rowMovementOffsets.push_back(1);

    rowMovementOffsets.push_back(2);
    rowMovementOffsets.push_back(1);

    rowMovementOffsets.push_back(-2);
    rowMovementOffsets.push_back(-1);

    rowMovementOffsets.push_back(-2);
    rowMovementOffsets.push_back(-1);

    //****

    columnMovementOffsets.push_back(1);
    columnMovementOffsets.push_back(2);
    
    columnMovementOffsets.push_back(-1);
    columnMovementOffsets.push_back(-2);

    columnMovementOffsets.push_back(1);
    columnMovementOffsets.push_back(2);
    
    columnMovementOffsets.push_back(-1);
    columnMovementOffsets.push_back(-2);
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