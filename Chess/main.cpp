#include "Board.hpp"

Board findBlackMove(Board board) {
    std::vector<Board> possibleMoves;
    board.generateAllMoves(possibleMoves, Piece::TEAM::BLACK);

    if(possibleMoves.size() == 0) {
        Board b;
        b.clearBoard();

        b.pieces[64] = 50;

        return b;
    }

    size_t smallestBoardIndex = 0;
    for(size_t i = 1; i < possibleMoves.size(); i++) {
        if(possibleMoves.at(i).pieces[64] < possibleMoves.at(smallestBoardIndex).pieces[64])
            smallestBoardIndex = i;
    }

    return possibleMoves.at(smallestBoardIndex);
} 

std::vector<Board> minimaxWhiteHelper(Board board, int depth) {
    if(depth == 1) {
        std::vector<Board> v;
        v.push_back(findBlackMove(board));
        return v;
    }
    
    std::vector<Board> possibleMoves;
    board.generateAllMoves(possibleMoves, depth % 2 == 0 ? Piece::TEAM::WHITE : Piece::TEAM::BLACK);

    //There are no moves to make. Return a board with a score for mate or draw
    if(possibleMoves.size() == 0) {
        Board b;
        b.clearBoard();

        //Black has no moves
        if(depth % 2 == 1) {
            //Mate
            if(PieceFunctionality::getInstance().isKingInCheck(board.pieces2D, Piece::TEAM::BLACK)) {
                b.pieces[64] = 50;
            } else {//Draw
                b.pieces[64] = 0;
            }
        } else {
            //Mate
            if(PieceFunctionality::getInstance().isKingInCheck(board.pieces2D, Piece::TEAM::WHITE)) {
                b.pieces[64] = -50;
            } else {//Draw
                b.pieces[64] = 0;
            }
        }

        std::vector<Board> v;
        v.push_back(b);
        return v;
    }

    std::vector<std::vector<Board>> opponentLines;
    for(Board &b : possibleMoves)
        opponentLines.push_back(minimaxWhiteHelper(b, depth - 1));

    size_t bestLineIndex = 0;
    if(depth % 2 == 0) {
        for(size_t i = 0; i < opponentLines.size(); i++) {
            if(opponentLines.at(i).at(0).pieces[64] > opponentLines.at(bestLineIndex).at(0).pieces[64] &&
                opponentLines.at(i).size() < opponentLines.at(bestLineIndex).size()) {

                bestLineIndex = i;
            }
        }
    } else {
        for(size_t i = 0; i < opponentLines.size(); i++) {
            if(opponentLines.at(i).at(0).pieces[64] < opponentLines.at(bestLineIndex).at(0).pieces[64] &&
                opponentLines.at(i).size() < opponentLines.at(bestLineIndex).size()) {

                bestLineIndex = i;
            }
        }
    }

    std::vector<Board> v = opponentLines.at(bestLineIndex);
    v.push_back(possibleMoves.at(bestLineIndex));

    return v;
}

std::vector<Board> minimaxWhite(Board board, int moves) {
    std::vector<Board> boards;
    board.generateAllMoves(boards, Piece::TEAM::WHITE);

    std::vector<std::vector<Board>> lines;
    for(Board temp : boards) {
        lines.push_back(minimaxWhiteHelper(temp, (2 * moves) - 1));
    }

    size_t bestLineIndex = 0;
    for(size_t i = 0; i < lines.size(); i++) {
        if(lines.at(i).at(0).pieces[64] > lines.at(bestLineIndex).at(0).pieces[64] &&
            lines.at(i).size() < lines.at(bestLineIndex).size()) {

            bestLineIndex = i;
        }
    }

    std::vector<Board> v = lines.at(bestLineIndex);
    v.push_back(boards.at(bestLineIndex));

    return v;
}

int main() {
    Board b;
    // b.clearBoard();
    b.setBoard();

    // b.pieces2D[0][4] = Piece::TYPE::BLACK_KING;
    // b.pieces2D[1][4] = Piece::TYPE::BLACK_PAWN;
    // b.pieces2D[1][5] = Piece::TYPE::BLACK_PAWN;
    
    // b.pieces2D[3][0] = Piece::TYPE::WHITE_BISHOP;
    // b.pieces2D[7][3] = Piece::TYPE::WHITE_ROOK;

    // b.pieces2D[6][5] = Piece::TYPE::WHITE_PAWN;
    // b.pieces2D[6][6] = Piece::TYPE::WHITE_PAWN;
    // b.pieces2D[6][7] = Piece::TYPE::WHITE_PAWN;

    // b.pieces2D[7][6] = Piece::TYPE::WHITE_KING;


    // b.pieces2D[2][0] = Piece::TYPE::BLACK_KING;
    // b.pieces2D[2][2] = Piece::TYPE::WHITE_KING;

    // b.pieces2D[2][7] = Piece::TYPE::WHITE_QUEEN;

    for(Board board : minimaxWhite(b, 3)) {
        board.print();
    }

    b.print();

    // b.pieces2D[1][4] = Piece::TYPE::EMPTY;
    // b.pieces2D[6][3] = Piece::TYPE::EMPTY;

    // b.pieces2D[3][4] = Piece::TYPE::BLACK_PAWN;
    // b.pieces2D[4][3] = Piece::TYPE::WHITE_PAWN;

    // b.pieces2D[1][4] = Piece::TYPE::EMPTY;
    // b.pieces2D[0][5] = Piece::TYPE::EMPTY;

    // b.pieces2D[4][1] = Piece::TYPE::BLACK_BISHOP;

    // b.pieces2D[6][3] = Piece::TYPE::EMPTY;

    // minimaxWhite(b, 2).print();

    // Board blackMoved;
    // blackMoved.clearBoard();

    // blackMoved.pieces2D[3][0] = Piece::TYPE::BLACK_KING;
    // blackMoved.pieces2D[2][2] = Piece::TYPE::WHITE_KING;

    // blackMoved.pieces2D[5][4] = Piece::TYPE::WHITE_QUEEN;

    // blackMoved.print();

    // minimaxWhite(blackMoved, 2).print();

    // for(Board &board : boards)
    //     board.print();




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




    // long long count = 0;

    // std::vector<Board> boards;

    // for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         if(b.pieces2D[i][j] % 2 == 0) 
    //             PieceFunctionality::getInstance().generateBoards(b, boards, i, j);
    //     }
    // }

    // // for(Board &board : boards) {
    // //     board.print();
    // //     std::cout << "------------------------------" << std::endl;
    // // }

    // // count += boards.size();

    // for(Board &board : boards) {
    //     std::cout << "1st move:" << std::endl;
    //     board.print();

    //     std::vector<Board> layer2;

    //     for(int i = 0; i <= 7; i++) {
    //     for(int j = 0; j <= 7; j++) {
    //         if(board.pieces2D[i][j] % 2 == 1) 
    //             PieceFunctionality::getInstance().generateBoards(board, layer2, i, j);
    //     }}

    //     // count += layer2.size();

    //     for(Board &temp : layer2) {
    //         std::cout << "2nd move:" << std::endl;
    //         temp.print();

    //         std::vector<Board> layer3;

    //         for(int i = 0; i <= 7; i++) {
    //         for(int j = 0; j <= 7; j++) {
    //             if(temp.pieces2D[i][j] % 2 == 0) 
    //                 PieceFunctionality::getInstance().generateBoards(temp, layer3, i, j);
    //         }}

    //         // count += layer3.size();

    //         for(Board &temp2 : layer3) {
    //             std::cout << "3rd move:" << std::endl;
    //             temp2.print();

    //             std::vector<Board> layer4;

    //             for(int i = 0; i <= 7; i++) {
    //             for(int j = 0; j <= 7; j++) {
    //                 if(temp2.pieces2D[i][j] % 2 == 1) 
    //                     PieceFunctionality::getInstance().generateBoards(temp2, layer4, i, j);
    //             }}

    //             count += layer4.size();

    //             // for(Board &temp3 : layer4) {
    //             //     // std::cout << "4th move:" << std::endl;
    //             //     // temp3.print();

    //             //     std::vector<Board> layer5;

    //             //     for(int i = 0; i <= 7; i++) {
    //             //     for(int j = 0; j <= 7; j++) {
    //             //         if(temp3.pieces2D[i][j] % 2 == 0) 
    //             //             PieceFunctionality::getInstance().generateBoards(temp3, layer5, i, j);
    //             //     }}

    //             //     count += layer5.size();
    //             // }
    //         }
    //     }

    // }

    // std::cout << count << std::endl;


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