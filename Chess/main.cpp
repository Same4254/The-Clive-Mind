#include "Board.hpp"

int main() {
    Board b;
    b.setBoard();

    std::vector<Board> boards;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(b.pieces2D[i][j] % 2 == 0)
                PieceFunctionality::getInstance().generateBoards(b, boards, i, j);
        }
    }

    for(Board &board : boards) {
        std::cout << "----------------------------------" << std::endl;
        board.print();
    }

    //Castle
    // b.pieces2D[0][0] = Piece::TYPE::EMPTY;
    // b.pieces2D[1][0] = Piece::TYPE::EMPTY;

    // // b.pieces2D[3][2] = Piece::TYPE::BLACK_ROOK;
    
    // b.pieces2D[7][1] = Piece::TYPE::EMPTY;
    // b.pieces2D[7][2] = Piece::TYPE::EMPTY;
    // b.pieces2D[7][3] = Piece::TYPE::EMPTY;

    // b.pieces2D[6][0] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][1] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][2] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][3] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][4] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][5] = Piece::TYPE::EMPTY;

    // b.print();
    // std::cout << "--------------------" << std::endl;

    // PieceFunctionality::getInstance().move(b.pieces2D, 7, 4, 7, 2);

    // b.printDebug();
    // std::cout << "--------------------" << std::endl;

    // En Passant
    // b.print();
    // std::cout << "--------------------" << std::endl;
    // PieceFunctionality::getInstance().move(b.pieces2D, 6, 4, 4, 4);

    // b.print();
    // std::cout << "--------------------" << std::endl;
    // PieceFunctionality::getInstance().move(b.pieces2D, 1, 0, 2, 0);

    // b.print();
    // std::cout << "--------------------" << std::endl;
    // PieceFunctionality::getInstance().move(b.pieces2D, 4, 4, 3, 4);

    // b.print();
    // std::cout << "--------------------" << std::endl;
    // PieceFunctionality::getInstance().move(b.pieces2D, 1, 5, 3, 5);

    // b.print();
    // std::cout << "--------------------" << std::endl;
    // PieceFunctionality::getInstance().move(b.pieces2D, 3, 4, 2, 5);

    // b.print();
    // std::cout << "--------------------" << std::endl;

    //Some Tests
    // b.clearBoard();
    // b.pieces2D[7][7] = Piece::TYPE::BLACK_KING;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 7, 7, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;

    // b.clearBoard();
    // b.pieces2D[4][4] = Piece::TYPE::BLACK_KING;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;

    // b.clearBoard();
    // b.pieces2D[4][4] = Piece::TYPE::BLACK_ROOK;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;

    // b.clearBoard();
    // b.pieces2D[4][4] = Piece::TYPE::BLACK_QUEEN;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;

    // b.clearBoard();
    // b.pieces2D[4][4] = Piece::TYPE::BLACK_KNIGHT;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 4, 4, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;

    // b.clearBoard();
    // b.pieces2D[0][0] = Piece::TYPE::BLACK_KNIGHT;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         std::cout << PieceFunctionality::getInstance().canMove(b.pieces2D, 0, 0, i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // b.print();

    // std::cout << "--------------------" << std::endl;
}