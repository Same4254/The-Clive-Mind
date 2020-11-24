#include "Pieces/Pawn/PawnDouble.hpp"
#include "Pieces/PieceFunctionality.hpp"

PawnDouble::PawnDouble(TEAM team, char displayCharacter) : Pawn(team, displayCharacter) {

}

PawnDouble::~PawnDouble() {
    
}

bool PawnDouble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(Pawn::canMove(pieces2D, startRow, startColumn, endRow, endColumn))
        return true;

    int nextRow = (team == Piece::TEAM::WHITE ? startRow - 1 : startRow + 1);
    int doubleRow = (team == Piece::TEAM::WHITE ? startRow - 2 : startRow + 2);

    if(endRow != doubleRow || pieces2D[nextRow][startColumn] != Piece::TYPE::EMPTY || pieces2D[endRow][startColumn] != Piece::TYPE::EMPTY)
        return false;

    return true;
}

bool PawnDouble::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;

        bool enPassant = abs(endRow - startRow) == 2;

        pieces2D[endRow][endColumn] = (enPassant ? (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE : Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE) :
                                                    (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_PAWN : Piece::TYPE::WHITE_PAWN));

        return true;
    }
    
    return false;
}