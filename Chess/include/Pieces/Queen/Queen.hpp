#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Pieces/Piece.hpp"

class Queen : public Piece {
public:
    Queen(TEAM team, char displayCharacter);
    virtual ~Queen();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual bool canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn);
};
#endif