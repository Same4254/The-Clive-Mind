#pragma once

#include "Pieces/BoardMovePieces/BoardMovePiece.hpp"

class Bishop : public BoardMovePiece {
public:
    Bishop(const PieceFunctionalityIndex piece, const char renderCharacter);
};