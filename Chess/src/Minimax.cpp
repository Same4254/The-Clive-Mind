#include "MiniMax.hpp"

Board MiniMax::findBlackMove(Board &board) {
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

Board MiniMax::evaluateWhiteMove(Board &board, int depth) {
    if(depth == 1) {
        Board b = findBlackMove(board);
        b.minimaxDepth = 1;
        return b;
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

        b.minimaxDepth = 1;
        return b;
    }

    std::vector<Board> opponentMoves;
    for(Board &b : possibleMoves)
        opponentMoves.push_back(evaluateWhiteMove(b, depth - 1));

    size_t bestLineIndex = 0;
    if(depth % 2 == 0) {
        for(size_t i = 0; i < opponentMoves.size(); i++) {
            if(opponentMoves.at(i).pieces[64] > opponentMoves.at(bestLineIndex).pieces[64] || (opponentMoves.at(i).pieces[64] == opponentMoves.at(bestLineIndex).pieces[64] && opponentMoves.at(i).minimaxDepth < opponentMoves.at(bestLineIndex).minimaxDepth)) {

                bestLineIndex = i;
            }
        }
    } else {
        for(size_t i = 0; i < opponentMoves.size(); i++) {
            if(opponentMoves.at(i).pieces[64] < opponentMoves.at(bestLineIndex).pieces[64] || (opponentMoves.at(i).pieces[64] == opponentMoves.at(bestLineIndex).pieces[64] && opponentMoves.at(i).minimaxDepth < opponentMoves.at(bestLineIndex).minimaxDepth)) {
                bestLineIndex = i;
            }
        }
    }

    Board b = opponentMoves.at(bestLineIndex);
    b.minimaxDepth++;

    return b;
}

Board MiniMax::bestWhiteMove(Board &board, int moves) {
    std::vector<Board> boards;
    board.generateAllMoves(boards, Piece::TEAM::WHITE);

    std::vector<Board> lines;
    for(Board &temp : boards) {
        lines.push_back(evaluateWhiteMove(temp, (2 * moves) - 1));
    }

    size_t bestLineIndex = 0;
    for(size_t i = 0; i < lines.size(); i++) {
        if(lines.at(i).pieces[64] > lines.at(bestLineIndex).pieces[64] || (lines.at(i).pieces[64] == lines.at(bestLineIndex).pieces[64] && lines.at(i).minimaxDepth < lines.at(bestLineIndex).minimaxDepth)) {

            bestLineIndex = i;
        }
    }

    return boards.at(bestLineIndex);
}