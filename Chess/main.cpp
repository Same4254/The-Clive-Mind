#include "Board.hpp"

int main() {
    Board b;
    b.clearBoard();
    b.pieces2D[7][7] = Piece::TYPE::BLACK_KING;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 7, 7, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;

    b.clearBoard();
    b.pieces2D[4][4] = Piece::TYPE::BLACK_KING;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;

    b.clearBoard();
    b.pieces2D[4][4] = Piece::TYPE::BLACK_ROOK;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;

    b.clearBoard();
    b.pieces2D[4][4] = Piece::TYPE::BLACK_QUEEN;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;

    b.clearBoard();
    b.pieces2D[4][4] = Piece::TYPE::BLACK_KNIGHT;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;

    b.clearBoard();
    b.pieces2D[0][0] = Piece::TYPE::BLACK_KNIGHT;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 0, 0, i, j) << " ";
        }
        std::cout << std::endl;
    }

    b.print();

    std::cout << "--------------------" << std::endl;
}