#include "Board.hpp"

int main() {
    Board b;
    b.setBoard();

    // b.clearBoard();

    // b.pieces2D[3][3] = Piece::TYPE::WHITE_KING;
    // b.pieces2D[3][4] = Piece::TYPE::WHITE_PAWN;

    // b.pieces2D[2][7] = Piece::TYPE::BLACK_KING;

    // // b.pieces2D[3][7] = Piece::TYPE::BLACK_ROOK;
    // // b.pieces2D[4][7] = Piece::TYPE::BLACK_ROOK;

    // b.pieces2D[0][6] = Piece::TYPE::BLACK_BISHOP;
    // b.pieces2D[2][1] = Piece::TYPE::BLACK_KNIGHT;
    
    // std::cout << "The situation:" << std::endl;

    // b.print();

    // std::cout << "------------------------------" << std::endl;

    // std::vector<Board> boards;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         if(b.pieces2D[i][j] % 2 == 0) 
    //             PieceFunctionality::getInstance().generateBoards(b, boards, i, j);
    //     }
    // }

    // for(Board &board : boards) {
    //     board.print();
    //     std::cout << "------------------------------" << std::endl;
    // }




    long long count = 0;

    std::vector<Board> boards;

    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(b.pieces2D[i][j] % 2 == 0) 
                PieceFunctionality::getInstance().generateBoards(b, boards, i, j);
        }
    }

    // for(Board &board : boards) {
    //     board.print();
    //     std::cout << "------------------------------" << std::endl;
    // }

    count += boards.size();

    for(Board &board : boards) {
        std::vector<Board> layer2;

        for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            if(board.pieces2D[i][j] % 2 == 0) 
                PieceFunctionality::getInstance().generateBoards(board, layer2, i, j);
        }}

        count += layer2.size();

        for(Board &temp : layer2) {
            std::vector<Board> layer3;

            for(int i = 0; i <= 7; i++) {
            for(int j = 0; j <= 7; j++) {
                if(temp.pieces2D[i][j] % 2 == 0) 
                    PieceFunctionality::getInstance().generateBoards(temp, layer3, i, j);
            }}

            count += layer3.size();

            for(Board &temp2 : layer3) {
                std::vector<Board> layer4;

                for(int i = 0; i <= 7; i++) {
                for(int j = 0; j <= 7; j++) {
                    if(temp2.pieces2D[i][j] % 2 == 0) 
                        PieceFunctionality::getInstance().generateBoards(temp2, layer4, i, j);
                }}

                count += layer4.size();

                for(Board &temp3 : layer4) {
                    std::vector<Board> layer5;

                    for(int i = 0; i <= 7; i++) {
                    for(int j = 0; j <= 7; j++) {
                        if(temp3.pieces2D[i][j] % 2 == 0) 
                            PieceFunctionality::getInstance().generateBoards(temp3, layer5, i, j);
                    }}

                    count += layer5.size();
                }
            }
        }

    }

    std::cout << count << std::endl;


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