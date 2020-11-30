#ifndef MINI_MAX_HPP
#define MINI_MAX_HPP

#include "Board.hpp"

class MiniMax {
private:
    static Board findBlackMove(Board &board);

public:
    static Board evaluateWhiteMove(Board &board, int depth);
    static Board bestWhiteMove(Board &board, int moves);
};
#endif