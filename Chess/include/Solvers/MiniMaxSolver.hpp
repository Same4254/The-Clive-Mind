#pragma once

#include "Types.hpp"

#include "BoardState.hpp"
#include "BoardFunctionality.hpp"

class MinimaxSolver {
public:
    Move solve(BoardState &boardState, const BoardFunctionality &boardFunctionality, uint32_t depth);
};