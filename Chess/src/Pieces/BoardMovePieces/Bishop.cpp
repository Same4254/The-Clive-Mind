#include "Pieces/BoardMovePieces/Bishop.hpp"

Bishop::Bishop(const PieceFunctionalityIndex piece, const char renderCharacter) : BoardMovePiece(piece, piece, renderCharacter) {
    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(-pos, pos));

    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(pos, pos));
    
    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(pos, -pos));

    for(int pos = 1; pos < BoardState::nRows; pos++)
        moveOffsets.push_back(std::make_pair(-pos, -pos));
}