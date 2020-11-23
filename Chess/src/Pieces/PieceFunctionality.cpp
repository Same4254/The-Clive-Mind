#include "Pieces/PieceFunctionality.hpp"

PieceFunctionality::PieceFunctionality() {
    //Can't get the length of an enum in C++ ...
    pieces = (Piece**) malloc(sizeof(Piece*) * 21);

    pieces[Piece::TYPE::EMPTY] = new EmptyPiece('-');

    //****** BLACK
    pieces[Piece::TYPE::BLACK_KING] = new King(Piece::TEAM::BLACK, 'K');
    pieces[Piece::TYPE::BLACK_KING_CASTLE_ABLE] = new KingCastleAble(Piece::TEAM::BLACK, 'K');

    pieces[Piece::TYPE::BLACK_PAWN] = new Pawn(Piece::TEAM::BLACK, 'P');
    pieces[Piece::TYPE::BLACK_PAWN_DOUBLE] = new PawnDouble(Piece::TEAM::BLACK, 'P');
    pieces[Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE] = new PawnEnPassantAble(Piece::TEAM::BLACK, 'P');

    pieces[Piece::TYPE::BLACK_ROOK] = new Rook(Piece::TEAM::BLACK, 'R');
    pieces[Piece::TYPE::BLACK_ROOK_CASTLE_ABLE] = new RookCastleAble(Piece::TEAM::BLACK, 'R');
    
    pieces[Piece::TYPE::BLACK_QUEEN] = new Queen(Piece::TEAM::BLACK, 'Q');
    pieces[Piece::TYPE::BLACK_BISHOP] = new Bishop(Piece::TEAM::BLACK, 'B');
    pieces[Piece::TYPE::BLACK_KNIGHT] = new Knight(Piece::TEAM::BLACK, 'N');

    //****** WHITE
    pieces[Piece::TYPE::WHITE_KING] = new King(Piece::TEAM::WHITE, 'k');
    pieces[Piece::TYPE::WHITE_KING_CASTLE_ABLE] = new KingCastleAble(Piece::TEAM::WHITE, 'k');

    pieces[Piece::TYPE::WHITE_PAWN] = new Pawn(Piece::TEAM::WHITE, 'p');
    pieces[Piece::TYPE::WHITE_PAWN_DOUBLE] = new PawnDouble(Piece::TEAM::WHITE, 'p');
    pieces[Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE] = new PawnEnPassantAble(Piece::TEAM::WHITE, 'p');

    pieces[Piece::TYPE::WHITE_ROOK] = new Rook(Piece::TEAM::WHITE, 'r');
    pieces[Piece::TYPE::WHITE_ROOK_CASTLE_ABLE] = new RookCastleAble(Piece::TEAM::WHITE, 'r');
    
    pieces[Piece::TYPE::WHITE_QUEEN] = new Queen(Piece::TEAM::WHITE, 'q');
    pieces[Piece::TYPE::WHITE_BISHOP] = new Bishop(Piece::TEAM::WHITE, 'b');
    pieces[Piece::TYPE::WHITE_KNIGHT] = new Knight(Piece::TEAM::WHITE, 'n');
}

PieceFunctionality::~PieceFunctionality() {
    for(uint i = 0; i < 21; i++)
        delete pieces[i];

    free(pieces);
}

/**
 * Given a piece on the board, check if that piece can be moved to the end location
 * 
 * @param startRow -> start row coordinate of the piece
 * @param startCol -> start col coordinate of the piece
 * @param endRow -> end row coordinate of the piece
 * @param endCol -> end col coordinate of the piece
 */
bool PieceFunctionality::canMove(PieceIndexType **pieces2D, uint startRow, uint startColumn, uint endRow, uint endColumn) {
    return pieces[pieces2D[startRow][startColumn]]->canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}

char PieceFunctionality::getDisplayCharacter(PieceIndexType pieceIndex) {
    return pieces[pieceIndex]->displayCharacter;
}