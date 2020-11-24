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

        BLACK_KING_CASTLE_ABLE, // 3
        WHITE_KING_CASTLE_ABLE, // 4

        BLACK_PAWN, // 5
        WHITE_PAWN, // 6

        BLACK_PAWN_DOUBLE, // 7
        WHITE_PAWN_DOUBLE, // 8

        BLACK_PAWN_EN_PASSANT_ABLE, // 9
        WHITE_PAWN_EN_PASSANT_ABLE, // 10

        BLACK_ROOK, // 11
        WHITE_ROOK, // 12

        BLACK_ROOK_CASTLE_ABLE, // 13
        WHITE_ROOK_CASTLE_ABLE, // 14

        BLACK_QUEEN, // 15
        WHITE_QUEEN, // 16

        BLACK_BISHOP, // 17
        WHITE_BISHOP, // 18

        BLACK_KNIGHT, // 19
        WHITE_KNIGHT // 20
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
    virtual bool move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) = 0;
};

#endif