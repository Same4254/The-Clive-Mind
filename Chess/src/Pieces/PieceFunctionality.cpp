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
    pieceFunctions = (Piece**) malloc(sizeof(Piece*) * 21);

    pieceFunctions[Piece::TYPE::EMPTY] = new EmptyPiece('-');

    //****** BLACK
    pieceFunctions[Piece::TYPE::BLACK_KING] = new King(Piece::TEAM::BLACK, 'K');
    pieceFunctions[Piece::TYPE::BLACK_KING_CASTLE_ABLE] = new KingCastleAble(Piece::TEAM::BLACK, 'K');

    pieceFunctions[Piece::TYPE::BLACK_PAWN] = new Pawn(Piece::TEAM::BLACK, 'P');
    pieceFunctions[Piece::TYPE::BLACK_PAWN_DOUBLE] = new PawnDouble(Piece::TEAM::BLACK, 'P');
    pieceFunctions[Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE] = new PawnEnPassantAble(Piece::TEAM::BLACK, 'P');

    pieceFunctions[Piece::TYPE::BLACK_ROOK] = new Rook(Piece::TEAM::BLACK, 'R');
    pieceFunctions[Piece::TYPE::BLACK_ROOK_CASTLE_ABLE] = new RookCastleAble(Piece::TEAM::BLACK, 'R');
    
    pieceFunctions[Piece::TYPE::BLACK_QUEEN] = new Queen(Piece::TEAM::BLACK, 'Q');
    pieceFunctions[Piece::TYPE::BLACK_BISHOP] = new Bishop(Piece::TEAM::BLACK, 'B');
    pieceFunctions[Piece::TYPE::BLACK_KNIGHT] = new Knight(Piece::TEAM::BLACK, 'N');

    //****** WHITE
    pieceFunctions[Piece::TYPE::WHITE_KING] = new King(Piece::TEAM::WHITE, 'k');
    pieceFunctions[Piece::TYPE::WHITE_KING_CASTLE_ABLE] = new KingCastleAble(Piece::TEAM::WHITE, 'k');

    pieceFunctions[Piece::TYPE::WHITE_PAWN] = new Pawn(Piece::TEAM::WHITE, 'w');
    pieceFunctions[Piece::TYPE::WHITE_PAWN_DOUBLE] = new PawnDouble(Piece::TEAM::WHITE, 'w');
    pieceFunctions[Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE] = new PawnEnPassantAble(Piece::TEAM::WHITE, 'w');

    pieceFunctions[Piece::TYPE::WHITE_ROOK] = new Rook(Piece::TEAM::WHITE, 'r');
    pieceFunctions[Piece::TYPE::WHITE_ROOK_CASTLE_ABLE] = new RookCastleAble(Piece::TEAM::WHITE, 'r');
    
    pieceFunctions[Piece::TYPE::WHITE_QUEEN] = new Queen(Piece::TEAM::WHITE, 'q');
    pieceFunctions[Piece::TYPE::WHITE_BISHOP] = new Bishop(Piece::TEAM::WHITE, 'b');
    pieceFunctions[Piece::TYPE::WHITE_KNIGHT] = new Knight(Piece::TEAM::WHITE, 'n');
}

PieceFunctionality::~PieceFunctionality() {
    for(int i = 0; i < 21; i++)
        delete pieceFunctions[i];

    free(pieceFunctions);
}

bool PieceFunctionality::isKingInCheck(PieceIndexType **pieces2D, Piece::TEAM team) {
    PieceIndexType kingIndex = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_KING : Piece::TYPE::WHITE_KING);
    PieceIndexType kingCastleIndex = (team == Piece::TEAM::BLACK ? Piece::TYPE::BLACK_KING_CASTLE_ABLE : Piece::TYPE::WHITE_KING_CASTLE_ABLE);

    for(int i = 0; i <= 7; i++) {
    for(int j = 0; j <= 7; j++) {
        if(pieces2D[i][j] == kingIndex || pieces2D[i][j] == kingCastleIndex) {
            if(team == Piece::TEAM::WHITE) {
                if(j < 7 && i > 0 && (pieces2D[i - 1][j + 1] == Piece::TYPE::BLACK_PAWN ||
                                        pieces2D[i - 1][j + 1] == Piece::TYPE::BLACK_PAWN_DOUBLE ||
                                        pieces2D[i - 1][j + 1] == Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE)) {
                    return true;
                }

                if(j > 0 && i > 0 && (pieces2D[i - 1][j - 1] == Piece::TYPE::BLACK_PAWN ||
                                        pieces2D[i - 1][j - 1] == Piece::TYPE::BLACK_PAWN_DOUBLE ||
                                        pieces2D[i - 1][j - 1] == Piece::TYPE::BLACK_PAWN_EN_PASSANT_ABLE)) {
                    return true;
                }
            }

            if(team == Piece::TEAM::BLACK) {
                if(j < 7 && i > 0 && (pieces2D[i - 1][j + 1] == Piece::TYPE::WHITE_PAWN ||
                                        pieces2D[i - 1][j + 1] == Piece::TYPE::WHITE_PAWN_DOUBLE ||
                                        pieces2D[i - 1][j + 1] == Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE)) {
                    return true;
                }

                if(j > 0 && i > 0 && (pieces2D[i - 1][j - 1] == Piece::TYPE::WHITE_PAWN ||
                                        pieces2D[i - 1][j - 1] == Piece::TYPE::WHITE_PAWN_DOUBLE ||
                                        pieces2D[i - 1][j - 1] == Piece::TYPE::WHITE_PAWN_EN_PASSANT_ABLE)) {
                    return true;
                }
            }

            PieceIndexType enemyRook = (team == Piece::TEAM::BLACK ? Piece::TYPE::WHITE_ROOK : Piece::TYPE::BLACK_ROOK);
            PieceIndexType enemyCastleRook = (team == Piece::TEAM::BLACK ? Piece::TYPE::WHITE_ROOK_CASTLE_ABLE : Piece::TYPE::BLACK_ROOK_CASTLE_ABLE);
            PieceIndexType enemyQueen = (team == Piece::TEAM::BLACK ? Piece::TYPE::WHITE_QUEEN : Piece::TYPE::BLACK_QUEEN);

            PieceIndexType enemyBishop = (team == Piece::TEAM::BLACK ? Piece::TYPE::WHITE_BISHOP : Piece::TYPE::BLACK_BISHOP);
            PieceIndexType enemyKnight = (team == Piece::TEAM::BLACK ? Piece::TYPE::WHITE_KNIGHT : Piece::TYPE::BLACK_KNIGHT);

            for(int tempRow = i + 1; tempRow < 8; tempRow++) {
                if(pieces2D[tempRow][j] == Piece::TYPE::EMPTY)
                    continue;

                if(pieces2D[tempRow][j] == enemyRook || pieces2D[tempRow][j] == enemyCastleRook || pieces2D[tempRow][j] == enemyQueen)
                    return true;
                
                break;
            }

            for(int tempRow = i - 1; tempRow >= 0; tempRow--) {
                if(pieces2D[tempRow][j] == Piece::TYPE::EMPTY)
                    continue;

                if(pieces2D[tempRow][j] == enemyRook || pieces2D[tempRow][j] == enemyCastleRook || pieces2D[tempRow][j] == enemyQueen)
                    return true;
                
                break;
            }

            for(int tempColumn = i - 1; tempColumn >= 0; tempColumn--) {
                if(pieces2D[i][tempColumn] == Piece::TYPE::EMPTY)
                    continue;

                if(pieces2D[i][tempColumn] == enemyRook || pieces2D[i][tempColumn] == enemyCastleRook || pieces2D[i][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            for(int tempColumn = i + 1; tempColumn < 8; tempColumn++) {
                if(pieces2D[i][tempColumn] == Piece::TYPE::EMPTY)
                    continue;

                if(pieces2D[i][tempColumn] == enemyRook || pieces2D[i][tempColumn] == enemyCastleRook || pieces2D[i][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            int rowFactor = 1;
            int columnFactor = 1;

            int tempRow = i + rowFactor;
            int tempColumn = j + columnFactor;

            while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
                if(pieces2D[tempRow][tempColumn] == Piece::TYPE::EMPTY) {
                    tempRow += rowFactor;
                    tempColumn += columnFactor;

                    continue;
                }

                if(pieces2D[tempRow][tempColumn] == enemyBishop || pieces2D[tempRow][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            rowFactor = -1;
            columnFactor = 1;

            tempRow = i + rowFactor;
            tempColumn = j + columnFactor;

            while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
                if(pieces2D[tempRow][tempColumn] == Piece::TYPE::EMPTY) {
                    tempRow += rowFactor;
                    tempColumn += columnFactor;

                    continue;
                }

                if(pieces2D[tempRow][tempColumn] == enemyBishop || pieces2D[tempRow][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            rowFactor = 1;
            columnFactor = -1;

            tempRow = i + rowFactor;
            tempColumn = j + columnFactor;

            while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
                if(pieces2D[tempRow][tempColumn] == Piece::TYPE::EMPTY) {
                    tempRow += rowFactor;
                    tempColumn += columnFactor;

                    continue;
                }

                if(pieces2D[tempRow][tempColumn] == enemyBishop || pieces2D[tempRow][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            rowFactor = -1;
            columnFactor = -1;

            tempRow = i + rowFactor;
            tempColumn = j + columnFactor;

            while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
                if(pieces2D[tempRow][tempColumn] == Piece::TYPE::EMPTY) {
                    tempRow += rowFactor;
                    tempColumn += columnFactor;

                    continue;
                }

                if(pieces2D[tempRow][tempColumn] == enemyBishop || pieces2D[tempRow][tempColumn] == enemyQueen)
                    return true;
                
                break;
            }

            if(i - 2 >= 0 && j + 1 <= 7 && pieces2D[i - 2][j + 1] == enemyKnight)
                return true;
            
            if(i - 1 >= 0 && j + 2 <= 7 && pieces2D[i - 1][j + 2] == enemyKnight)
                return true;

            if(i - 2 >= 0 && j - 1 >= 0 && pieces2D[i - 2][j - 1] == enemyKnight)
                return true;
            
            if(i - 1 >= 0 && j - 2 >= 0 && pieces2D[i - 1][j - 2] == enemyKnight)
                return true;

            // ***

            if(i + 2 <= 7 && j - 1 >= 0 && pieces2D[i + 2][j - 1] == enemyKnight)
                return true;
            
            if(i + 1 <= 7 && j - 2 >= 0 && pieces2D[i + 1][j - 2] == enemyKnight)
                return true;

            if(i + 2 <= 7 && j + 1 <= 7 && pieces2D[i + 2][j + 1] == enemyKnight)
                return true;
            
            if(i + 1 <= 7 && j + 2 <= 7 && pieces2D[i + 1][j + 2] == enemyKnight)
                return true;

            return false;

            // bool inCheck = canAnyPieceMoveTo(pieces2D, i, j, (pieces2D[endRow][endColumn] % 2 == 0 ? Piece::TEAM::BLACK : Piece::TEAM::WHITE));

            // pieces2D[startRow][startColumn] = pieces2D[endRow][endColumn];
            // pieces2D[endRow][endColumn] = oldPieceAtTarget;

            // if(inCheck)
            //     return false;
            // return true;
        }
    }}

    return false;
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
        
        // std::cout << "Out of bounds on canMove, PieceFunctionality" << std::endl;
        
        return false;
    }

    if(pieceFunctions[pieces2D[startRow][startColumn]]->canMove(pieces2D, startRow, startColumn, endRow, endColumn)) {
        PieceIndexType oldPieceAtTarget = pieces2D[endRow][endColumn];

        pieces2D[endRow][endColumn] = pieces2D[startRow][startColumn];
        pieces2D[startRow][startColumn] = Piece::TYPE::EMPTY;

        bool inCheck = isKingInCheck(pieces2D, (pieces2D[endRow][endColumn] % 2 == 1 ? Piece::TEAM::BLACK : Piece::TEAM::WHITE));

        pieces2D[startRow][startColumn] = pieces2D[endRow][endColumn];
        pieces2D[endRow][endColumn] = oldPieceAtTarget;

        if(inCheck)
            return false;
        return true;
    }

    return false;
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

void PieceFunctionality::move(PieceIndexType **pieces2D, int startRow, int startColumn, int endRow, int endColumn) {
    if(startRow < 0 || startRow > 7 || startColumn < 0 || startColumn > 7 ||
        endRow < 0 || endRow > 7 || endColumn < 0 || endColumn > 7) {
        
        // std::cout << "Out of bounds on canMove, PieceFunctionality" << std::endl;
        return;
    }

    pieceFunctions[pieces2D[startRow][startColumn]]->move(pieces2D, startRow, startColumn, endRow, endColumn);
}

//TODO: This is not great, but works for now
void PieceFunctionality::generateBoards(Board &board, std::vector<Board> &boards, int pieceRow, int pieceColumn) {
    if(board.pieces2D[pieceRow][pieceColumn] == Piece::TYPE::EMPTY)
        return;

    // for(int i = 0; i <= 7; i++) {
    // for(int j = 0; j <= 7; j++) {
    //     if(canMove(board.pieces2D, pieceRow, pieceColumn, i, j)) {
    //         Board b(board);
    //         move(b.pieces2D, pieceRow, pieceColumn, i, j);
    //         boards.push_back(b);

    //         // boards.push_back(Board(board));
    //         // move(boards.at(boards.size() - 1).pieces2D, pieceRow, pieceColumn, i, j);
    //     }
    // }}

    pieceFunctions[board.pieces2D[pieceRow][pieceColumn]]->generateBoards(board, boards, pieceRow, pieceColumn);
}

char PieceFunctionality::getDisplayCharacter(PieceIndexType pieceIndex) {
    return pieceFunctions[pieceIndex]->displayCharacter;
}