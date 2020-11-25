#ifndef EMPTY_PIECE_HPP
#define EMPTY_PIECE_HPP

#include "Pieces/Piece.hpp"

class EmptyPiece : public Piece {
public:
    EmptyPiece(char displayCharacter);
    virtual ~EmptyPiece();

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
    virtual void move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);
};
#endif