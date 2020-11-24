#ifndef PIECE_HPP
#define PIECE_HPP

#include <cmath>

typedef unsigned char PieceIndexType;

class Piece {
public:
    //Black -> odd
    //White -> even
    enum TYPE {
        EMPTY, // 0
        
        BLACK_KING, // 1
        WHITE_KING, // 2

        BLACK_KING_CASTLE_ABLE,
        WHITE_KING_CASTLE_ABLE,

        BLACK_PAWN,
        WHITE_PAWN,

        BLACK_PAWN_DOUBLE,
        WHITE_PAWN_DOUBLE,

        BLACK_PAWN_EN_PASSANT_ABLE,
        WHITE_PAWN_EN_PASSANT_ABLE,

        BLACK_ROOK,
        WHITE_ROOK,

        BLACK_ROOK_CASTLE_ABLE,
        WHITE_ROOK_CASTLE_ABLE,

        BLACK_QUEEN,
        WHITE_QUEEN,

        BLACK_BISHOP,
        WHITE_BISHOP,

        BLACK_KNIGHT,
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
    virtual bool canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) = 0;
};

#endif