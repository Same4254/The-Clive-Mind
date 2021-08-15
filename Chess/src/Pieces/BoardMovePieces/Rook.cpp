#include "Pieces/BoardMovePieces/Rook.hpp"

Rook::Rook(const PieceFunctionalityIndex piece, const char renderCharacter) : BoardMovePiece(piece, piece, renderCharacter) {
    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(0, pos));

    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(pos, 0));
    
    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(0, -pos));

    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(-pos, 0));
}