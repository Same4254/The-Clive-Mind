#ifndef PIECE_HPP
#define PIECE_HPP

#include <memory>

typedef unsigned char PieceIndexType;

class Piece {
public:
    enum TYPE {
        EMPTY,

        //****** BLACK
        BLACK_KING,
        BLACK_KING_CASTLE_ABLE,

        BLACK_PAWN,
        BLACK_PAWN_DOUBLE,
        BLACK_PAWN_EN_PASSANT_ABLE,

        BLACK_ROOK,
        BLACK_ROOK_CASTLE_ABLE,

        BLACK_QUEEN,
        BLACK_BISHOP,
        BLACK_KNIGHT,

        //****** WHITE
        WHITE_KING,
        WHITE_KING_CASTLE_ABLE,

        WHITE_PAWN,
        WHITE_PAWN_DOUBLE,
        WHITE_PAWN_EN_PASSANT_ABLE,

        WHITE_ROOK,
        WHITE_ROOK_CASTLE_ABLE,

        WHITE_QUEEN,
        WHITE_BISHOP,
        WHITE_KNIGHT
    };

    enum TEAM {
        WHITE, 
        BLACK,
        NONE
    };

    TEAM team;
    char displayCharacter;

    Piece(TEAM team, char displayCharacter);
    virtual ~Piece();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual bool canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) = 0;
};

#endif