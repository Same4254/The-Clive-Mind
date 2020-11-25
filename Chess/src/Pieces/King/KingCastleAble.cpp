#include "Pieces/King/KingCastleAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

#include "Board.hpp"

KingCastleAble::KingCastleAble(TEAM team, char displayCharacter) : King(team, displayCharacter) {
    rowMovementOffsets.push_back(0);
    rowMovementOffsets.push_back(0);

    columnMovementOffsets.push_back(2);
    columnMovementOffsets.push_back(-2);
}

KingCastleAble::~KingCastleAble() {
    
}

bool KingCastleAble::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(King::canMove(pieces2D, startRow, startColumn, endRow, endColumn))
        return true;

    if(startRow != endRow)
        return false;

    TEAM oppositeTeam = (team == TEAM::WHITE ? TEAM::BLACK : TEAM::WHITE);
    PieceIndexType castleRookIndex = (team == TEAM::WHITE ? WHITE_ROOK_CASTLE_ABLE : BLACK_ROOK_CASTLE_ABLE);

    //King side castle
    if(endColumn == startColumn + 2) {
        if(pieces2D[startRow][startColumn + 1] != Piece::TYPE::EMPTY ||
            pieces2D[startRow][startColumn + 2] != Piece::TYPE::EMPTY ||
            pieces2D[startRow][startColumn + 3] != castleRookIndex)
            return false;

        if(PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn, oppositeTeam) ||
            PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn + 1, oppositeTeam) ||
            PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn + 2, oppositeTeam))
            return false;
        return true;
    } else if(endColumn == startColumn - 2){//Queen side castle
        if(pieces2D[startRow][startColumn - 1] != Piece::TYPE::EMPTY ||
            pieces2D[startRow][startColumn - 2] != Piece::TYPE::EMPTY ||
            pieces2D[startRow][startColumn - 4] != castleRookIndex)
            return false;

        if(PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn, oppositeTeam) ||
            PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn - 1, oppositeTeam) ||
            PieceFunctionality::getInstance().canAnyPieceMoveTo(pieces2D, startRow, startColumn - 2, oppositeTeam))
            return false;
        return true;
    }

    return false;
}

void KingCastleAble::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    // if(canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        if(endColumn == startColumn - 2) {//Queen side castle
            pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
            pieces2D[startRow][startColumn - 4] = Piece::TYPE::EMPTY;

            pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
            pieces2D[startRow][startColumn - 2] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_KING : Piece::TYPE::WHITE_KING);
            pieces2D[startRow][startColumn - 1] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_ROOK : Piece::TYPE::WHITE_ROOK);
        } else if(endColumn == startColumn + 2) {//King side castle
            pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
            pieces2D[startRow][startColumn + 3] = Piece::TYPE::EMPTY;

            pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
            pieces2D[startRow][startColumn + 2] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_KING : Piece::TYPE::WHITE_KING);
            pieces2D[startRow][startColumn + 1] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_ROOK : Piece::TYPE::WHITE_ROOK);
        } else {
            pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;
            pieces2D[endRow][endColumn] = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_KING : Piece::TYPE::WHITE_KING);
        }

    //     return true;
    // }
    // return false;
}