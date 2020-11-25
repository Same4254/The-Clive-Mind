#include "Pieces/Pawn/Pawn.hpp"
#include "Pieces/PieceFunctionality.hpp"

Pawn::Pawn(TEAM team, char displayCharacter) : Piece(team, displayCharacter) {
    if(team == Piece::TEAM::BLACK) {
        rowMovementOffsets.push_back(1);
        rowMovementOffsets.push_back(1);
        rowMovementOffsets.push_back(1);
    }

    if(team == Piece::TEAM::WHITE) {
        rowMovementOffsets.push_back(-1);
        rowMovementOffsets.push_back(-1);
        rowMovementOffsets.push_back(-1);
    }

    columnMovementOffsets.push_back(-1);
    columnMovementOffsets.push_back(0);
    columnMovementOffsets.push_back(1);
}

Pawn::~Pawn() {
    
}

bool Pawn::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    int nextRow = (team == Piece::TEAM::WHITE ? startRow - 1 : startRow + 1);

    if(endRow != nextRow)
        return false;

    //Push the pawn
    if(endColumn == startColumn && pieces2D[endRow][endColumn] == Piece::TYPE::EMPTY)
        return true;
    
    if(abs(endColumn - startColumn) == 1) {
        if(pieces2D[endRow][endColumn] != Piece::TYPE::EMPTY && pieces2D[startRow][startColumn] % 2 != pieces2D[endRow][endColumn] % 2)
            return true;
        
        PieceIndexType enPassantTakeable = (team == Piece::TEAM::WHITE ? Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE : Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE);
        if(pieces2D[endRow][endColumn] == Piece::TYPE::EMPTY && pieces2D[startRow][endColumn] == enPassantTakeable)
            return true;
    }

    return false;
}

void Pawn::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    // if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType temp = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
        pieces2D[endRow][endColumn] = temp;

        if(endColumn != startColumn) {
            pieces2D[startRow][endColumn] = Piece::TYPE::EMPTY;
        }

    //     return true;
    // }
    
    // return false;
}