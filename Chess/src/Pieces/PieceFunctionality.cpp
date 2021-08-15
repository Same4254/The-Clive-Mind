#include "Pieces/PieceFunctionality.hpp"

PieceFunctionality::PieceFunctionality(const PieceFunctionalityIndex piece, const char renderCharacter, const bool doesMove) 
    : piece(piece), teamModularity(piece % 2), renderCharacter(renderCharacter), doesMove(doesMove) 
{
    pieceValues[EMPTY] = 0;

    pieceValues[BLACK_KING] = -1000;
    pieceValues[WHITE_KING] = 1000;

    pieceValues[BLACK_KING_CASTLE_ABLE] = -1000;
    pieceValues[WHITE_KING_CASTLE_ABLE] = 1000;

    pieceValues[BLACK_PAWN] = -1;
    pieceValues[WHITE_PAWN] = 1;

    pieceValues[BLACK_PAWN_DOUBLE] = -1;
    pieceValues[WHITE_PAWN_DOUBLE] = 1;

    pieceValues[BLACK_ROOK] = -5;
    pieceValues[WHITE_ROOK] = 5;

    pieceValues[BLACK_ROOK_CASTLE_ABLE] = -5;
    pieceValues[WHITE_ROOK_CASTLE_ABLE] = 5;
    
    pieceValues[BLACK_QUEEN] = -9;
    pieceValues[WHITE_QUEEN] = 9;
    
    pieceValues[BLACK_BISHOP] = -3;
    pieceValues[WHITE_BISHOP] = 3;
    
    pieceValues[BLACK_KNIGHT] = -3;
    pieceValues[WHITE_KNIGHT] = 3;
}