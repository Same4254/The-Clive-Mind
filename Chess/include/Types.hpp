#pragma once

#include <stdint.h>

//The type that represents the hash of a board
typedef uint64_t BoardHash;

//This is the type of integer on the board that represent the index of the functionality object
typedef uint32_t PieceFunctionalityIndex;

//The index used to access the board pieces
typedef int32_t BoardPieceIndex;

//Index that tells what move for the piece to make
typedef int MoveIndex;

//Score of the game
typedef int Score;

#define MAX_MOVES_PER_PIECE 28

//Sets up the modularity 
enum TEAM {
    WHITE, // 0
    BLACK  // 1
};

enum PIECES {
    EMPTY, // 0
        
    BLACK_KING, // 1
    WHITE_KING, // 2

    BLACK_KING_CASTLE_ABLE, // 3
    WHITE_KING_CASTLE_ABLE, // 4

    BLACK_PAWN, // 5
    WHITE_PAWN, // 6

    BLACK_PAWN_DOUBLE, // 7
    WHITE_PAWN_DOUBLE, // 8

    BLACK_ROOK, // 9
    WHITE_ROOK, // 10

    BLACK_ROOK_CASTLE_ABLE, // 11
    WHITE_ROOK_CASTLE_ABLE, // 12

    BLACK_QUEEN, // 13
    WHITE_QUEEN, // 14

    BLACK_BISHOP, // 15
    WHITE_BISHOP, // 16

    BLACK_KNIGHT, // 17
    WHITE_KNIGHT, // 18

    //Keep this at the bottom so the amount of pieces is known
    NUM_PIECES
};