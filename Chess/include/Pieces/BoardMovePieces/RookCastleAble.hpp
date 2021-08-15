#pragma once

#include "Pieces/BoardMovePieces/BoardMovePiece.hpp"

class RookCastleAble : public BoardMovePiece {
public:
    RookCastleAble(const PieceFunctionalityIndex piece, const char renderCharacter);
};