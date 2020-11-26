#include "Board.hpp"

Board::Board() {
    //Chess board is 8x8 board. Thus, 64 indecies long for the board. In addition, one more for the score
    pieces = (PieceIndexType*) malloc((sizeof(PieceIndexType) * 65));
    pieces2D = (PieceIndexType**) malloc((sizeof(PieceIndexType*) * 8));

    //Set the row pointer to the pointer at the location in the 1D allocation
    for(int i = 0; i < 8; i++)
        pieces2D[i] = &pieces[i * 8];
}

Board::Board(const Board &other) : Board() {
    memcpy(pieces, other.pieces, sizeof(PieceIndexType) * 65);
}

Board::~Board() {
    free(pieces);
    free(pieces2D);
}

void Board::setBoard() {
    pieces[64] = 0;

    pieces2D[0][0] = Piece::TYPE::BLACK_ROOK_CASTLE_ABLE;
    pieces2D[0][1] = Piece::TYPE::BLACK_KNIGHT;
    pieces2D[0][2] = Piece::TYPE::BLACK_BISHOP;
    pieces2D[0][3] = Piece::TYPE::BLACK_QUEEN;
    pieces2D[0][4] = Piece::TYPE::BLACK_KING_CASTLE_ABLE;
    pieces2D[0][5] = Piece::TYPE::BLACK_BISHOP;
    pieces2D[0][6] = Piece::TYPE::BLACK_KNIGHT;
    pieces2D[0][7] = Piece::TYPE::BLACK_ROOK_CASTLE_ABLE;

    for(int i = 0; i <= 7; i++)
        pieces2D[1][i] = Piece::TYPE::BLACK_PAWN_DOUBLE;

    for(int i = 2; i <= 5; i++) {
        for(int j = 0; j <= 7; j++) {
            pieces2D[i][j] = Piece::TYPE::EMPTY;
        }
    }

    for(int i = 0; i <= 7; i++)
        pieces2D[6][i] = Piece::TYPE::WHITE_PAWN_DOUBLE;

    pieces2D[7][0] = Piece::TYPE::WHITE_ROOK_CASTLE_ABLE;
    pieces2D[7][1] = Piece::TYPE::WHITE_KNIGHT;
    pieces2D[7][2] = Piece::TYPE::WHITE_BISHOP;
    pieces2D[7][3] = Piece::TYPE::WHITE_QUEEN;
    pieces2D[7][4] = Piece::TYPE::WHITE_KING_CASTLE_ABLE;
    pieces2D[7][5] = Piece::TYPE::WHITE_BISHOP;
    pieces2D[7][6] = Piece::TYPE::WHITE_KNIGHT;
    pieces2D[7][7] = Piece::TYPE::WHITE_ROOK_CASTLE_ABLE;
}

void Board::clearBoard() {
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            pieces2D[i][j] = Piece::TYPE::EMPTY;
        }
    }
}

bool Board::canMove(int startRow, int startColumn, int endRow, int endColumn) {
    return PieceFunctionality::getInstance().canMove(pieces2D, startRow, startColumn, endRow, endColumn);
}

bool Board::canBeMovedOnto(int row, int col) {
    for(int tempRow = 0; tempRow < 8; tempRow++) {
        for(int tempCol = 0; tempCol < 8; tempCol++) {
            if(tempRow == row && tempCol == col) 
                continue;

            if(canMove(tempRow, tempCol, row, col))
                return true;
        }
    }

    return false;
} 

void Board::print() {
    for(int row = 0; row < 8; row++) {
        std::cout << 8 - row << " | ";
        for(int column = 0; column < 8; column++) {
            std::cout << PieceFunctionality::getInstance().getDisplayCharacter(pieces2D[row][column]) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "   ---------------- Score: " << (int) pieces[64] << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
}

void Board::printDebug() {
    for(int row = 0; row < 8; row++) {
        std::cout << 8 - row << " | ";
        for(int column = 0; column < 8; column++) {
            std::cout << std::setfill('0') << std::setw(2) << (int) pieces2D[row][column] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "   ------------------------" << std::endl;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
}