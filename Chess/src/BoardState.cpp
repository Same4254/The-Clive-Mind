#include "BoardState.hpp"

void BoardState::set2DPointers() {
    for(uint32_t i = 0; i < nRows; i++)
        pieces2D[i] = &pieces[i * nCols];
}