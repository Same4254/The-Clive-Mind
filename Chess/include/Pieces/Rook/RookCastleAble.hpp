#ifndef ROOK_CASTLE_ABLE_HPP
#define ROOK_CASTLE_ABLE_HPP

#include "Pieces/Piece.hpp"
#include "Pieces/Rook/Rook.hpp"

class RookCastleAble : public Rook {
public:
    RookCastleAble(TEAM team, char displayCharacter);
    virtual ~RookCastleAble();

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
     */
    virtual void move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

};
#endif