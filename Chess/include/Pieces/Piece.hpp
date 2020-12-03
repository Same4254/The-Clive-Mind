#ifndef PIECE_HPP
#define PIECE_HPP

#include <cmath>
#include <vector>

//Type of the indecies in IDs in the board
typedef int PieceIndexType;

//Circle Dependency
class Board;

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

protected:
    TEAM team;
    char displayCharacter;

    /**
     * Every piece has position offsets that describe its movement. 
     * When possible, these lists are used to generalize piece movement, but this is not always possible
     */
    std::vector<int> rowMovementOffsets, columnMovementOffsets;

public:
    Piece(TEAM team, char displayCharacter);
    virtual ~Piece();

    /**
     * Given a piece on the board, check if that piece can be moved to the end location
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     * 
     * @return -> Whether or not the piece at the given position, on the given board, can move to the target location
     */
    virtual bool canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) = 0;

    /**
     * Given a piece on the board, move the piece to the end location.
     * 
     * WARNING. This does not call canMove before moving the piece. This is so that when generating the board, 
     *      boards are only created when the piece can move, and then when it is moved, it doesn't check the move again
     * 
     * @param startRow -> start row coordinate of the piece
     * @param startCol -> start col coordinate of the piece
     * @param endRow -> end row coordinate of the piece
     * @param endCol -> end col coordinate of the piece
     */
    virtual void move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn);

    /**
     * Given a board, generate all of the possible boards (as a result of all possible movements of the piece at the location). 
     * These possible boards are added into the boards vector.
     * 
     * @param board -> The board to generate moves from
     * @param boards -> The generated list of boards to add to
     * @param pieceRow -> Row of the piece to try all moves for
     * @param pieceColumn -> Column of the piece to try all moves for
     */
    virtual void generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn);

    //**** GETTERS & SETTERS
    virtual TEAM getTeam();
    virtual char getDisplayCharacter();
};

#endif