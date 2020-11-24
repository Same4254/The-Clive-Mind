#ifndef KING_CASTLE_ABLE_HPP
#define KING_CASTLE_ABLE_HPP

#include "Pieces/Piece.hpp"
#include "Pieces/King/King.hpp"

class KingCastleAble : public King {
public:
    KingCastleAble(TEAM team, char displayCharacter);
    virtual ~KingCastleAble();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual bool canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

    /**
     * Given a piece on the board, move the piece to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     * 
     * @return -> Whether or not the piece was moved
     */
    virtual bool move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);
};
#endif