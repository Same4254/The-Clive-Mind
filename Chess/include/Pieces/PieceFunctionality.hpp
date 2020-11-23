#ifndef PIECE_FUNCTIONALITY_HPP
#define PIECE_FUNCTIONALITY_HPP

#include <memory>
#include "Pieces/Piece.hpp"

#include "Pieces/EmptyPiece.hpp"

#include "Pieces/Bishop/Bishop.hpp"
#include "Pieces/King/King.hpp"
#include "Pieces/King/KingCastleAble.hpp"
#include "Pieces/Knight/Knight.hpp"
#include "Pieces/Pawn/Pawn.hpp"
#include "Pieces/Pawn/PawnDouble.hpp"
#include "Pieces/Pawn/PawnEnPassantAble.hpp"
#include "Pieces/Queen/Queen.hpp"
#include "Pieces/Rook/Rook.hpp"
#include "Pieces/Rook/RookCastleAble.hpp"

class PieceFunctionality {
private:
    Piece **pieces;

    PieceFunctionality();
    ~PieceFunctionality();

public:
    static PieceFunctionality& getInstance() {
        static PieceFunctionality instance;
        return instance;
    }

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    bool canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn);

    char getDisplayCharacter(PieceIndexType pieceIndex);
};

#endif