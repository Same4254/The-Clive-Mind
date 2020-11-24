#include "Pieces/PieceFunctionality.hpp"

#include "Pieces/Piece.hpp"
#include "Board.hpp"

#include "Pieces/EmptyPiece.hpp"

#include "Pieces/Bishop/Bishop.hpp"
#include "Pieces/King/King.hpp"
#include "Pieces/King/KingCastleAble.hpp"
#include "Pieces/Knight/Knight.hpp"
#include "Pieces/Pawn/Pawn.hpp"
#include "Pieces/Pawn/PawnDouble.hpp"
#include "Pieces/Pawn/PawnEnPassantAble.hpp"
#include "Pieces/Queen/Queen.hpp"
#include "Pieces/Rook/Rook.hpp"
#include "Pieces/Rook/RookCastleAble.hpp"

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

    pieces[Piece::TYPE::WHITE_PAWN] = new Pawn(Piece::TEAM::WHITE, 'w');
    pieces[Piece::TYPE::WHITE_PAWN_DOUBLE] = new PawnDouble(Piece::TEAM::WHITE, 'w');
    pieces[Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE] = new PawnEnPassantAble(Piece::TEAM::WHITE, 'w');

    pieces[Piece::TYPE::WHITE_ROOK] = new Rook(Piece::TEAM::WHITE, 'r');
    pieces[Piece::TYPE::WHITE_ROOK_CASTLE_ABLE] = new RookCastleAble(Piece::TEAM::WHITE, 'r');
    
    pieces[Piece::TYPE::WHITE_QUEEN] = new Queen(Piece::TEAM::WHITE, 'q');
    pieces[Piece::TYPE::WHITE_BISHOP] = new Bishop(Piece::TEAM::WHITE, 'b');
    pieces[Piece::TYPE::WHITE_KNIGHT] = new Knight(Piece::TEAM::WHITE, 'n');
}

PieceFunctionality::~PieceFunctionality() {
    for(int i = 0; i < 21; i++)
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
bool PieceFunctionality::canMove(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(startRow < 0 || startRow > 7 || startColumn < 0 || startColumn > 7 ||
        endRow < 0 || endRow > 7 || endColumn < 0 || endColumn > 7) {
        
        std::cout << "Out of bounds on canMove, PieceFunctionality" << std::endl;
        return false;
    }

    return pieces[pieces2D[startRow][startColumn]]->canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}

bool PieceFunctionality::canAnyPieceMoveTo(PieceIndexType **pieces2D, int row, int column, Piece::TEAM team) {
    for(int tempRow = 0; tempRow < 8; tempRow++) {
        for(int tempCol = 0; tempCol < 8; tempCol++) {
            if(tempRow == row && tempCol == column) 
                continue;

            PieceIndexType piece = pieces2D[tempRow][tempCol];
            if(piece != Piece::EMPTY && piece % 2 == team && canMove(pieces2D, tempRow, tempCol, row, column))
                return true;
        }
    }

    return false;
}

bool PieceFunctionality::canAnyWhitePieceMoveTo(PieceIndexType **pieces2D, int row, int column) {
    return canAnyPieceMoveTo(pieces2D, row, column, Piece::TEAM::WHITE);
}

bool PieceFunctionality::canAnyBlackPieceMoveTo(PieceIndexType **pieces2D, int row, int column) {
    return canAnyPieceMoveTo(pieces2D, row, column, Piece::TEAM::BLACK);
}

bool PieceFunctionality::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(startRow < 0 || startRow > 7 || startColumn < 0 || startColumn > 7 ||
        endRow < 0 || endRow > 7 || endColumn < 0 || endColumn > 7) {
        
        std::cout << "Out of bounds on canMove, PieceFunctionality" << std::endl;
        return false;
    }

    return pieces[pieces2D[startRow][startColumn]]->move(pieces2D, startRow, startColumn, endRow, endColumn);
}

//TODO: This is not great, but works for now
void PieceFunctionality::generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn) {
    if(board.pieces2D[pieceRow][pieceColumn] == Piece::TYPE::EMPTY)
        return;

    for(int i = 0; i <= 7; i++) {
    for(int j = 0; j <= 7; j++) {
        if(canMove(board.pieces2D, pieceRow, pieceColumn, i, j)) {
            Board b(board);
            move(b.pieces2D, pieceRow, pieceColumn, i, j);
            boards.push_back(b);

            // boards.push_back(Board(board));
            // move(boards.at(boards.size() - 1).pieces2D, pieceRow, pieceColumn, i, j);
        }
    }}
}

char PieceFunctionality::getDisplayCharacter(PieceIndexType pieceIndex) {
    return pieces[pieceIndex]->displayCharacter;
}