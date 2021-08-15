#pragma once

#include "Pieces/BoardMovePieces/BoardMovePiece.hpp"

class Rook : public BoardMovePiece {
public:
    Rook(const PieceFunctionalityIndex piece, const char renderCharacter);
};