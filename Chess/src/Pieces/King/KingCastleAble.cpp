#include "Pieces/King/KingCastleAble.hpp"
#include "Pieces/PieceFunctionality.hpp"

KingCastleAble::KingCastleAble(TEAM team, char displayCharacter) : King(team, displayCharacter) {

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