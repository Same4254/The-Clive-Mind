#pragma once

#include <stdint.h>
#include <vector>
#include <array>

#include "Types.hpp"

class SquareInfo {
public:
    BoardPieceIndex row;
    BoardPieceIndex col;
    PieceFunctionalityIndex oldPiece;
    PieceFunctionalityIndex newPiece;
};

class Move {
public:
    int32_t score;
    uint32_t depth;

    //Amount of peices that are moved in this move
    uint32_t amtSquaresChanged;

    //At most 5 squares changed (castle while a pawn loses en passent)
    std::array<SquareInfo, 5> squaresChanged;

    // friend bool operator<(const Move &move1, const Move &move2);
    inline bool operator<(const Move &move2) {
        if(score == move2.score)
            return move2.depth < depth;
        else
            return score < move2.score;
    }
};