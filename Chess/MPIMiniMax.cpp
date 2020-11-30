#define MPI_H
#include <mpi.h>

#include <unordered_map>

#include "MiniMax.hpp"

int main() {
    int id;
    int worldSize;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int moveDepth = 3;

    if(id == 0) {
        Board board;
        // board.setBoard();
        board.clearBoard();
        board.pieces2D[0][4] = Piece::TYPE::BLACK_KING;
        board.pieces2D[1][4] = Piece::TYPE::BLACK_PAWN;
        board.pieces2D[1][5] = Piece::TYPE::BLACK_PAWN;
        
        board.pieces2D[3][0] = Piece::TYPE::WHITE_BISHOP;
        board.pieces2D[7][3] = Piece::TYPE::WHITE_ROOK;

        board.pieces2D[6][5] = Piece::TYPE::WHITE_PAWN;
        board.pieces2D[6][6] = Piece::TYPE::WHITE_PAWN;
        board.pieces2D[6][7] = Piece::TYPE::WHITE_PAWN;

        board.pieces2D[7][6] = Piece::TYPE::WHITE_KING;

        std::vector<Board> boardsToEvalutate;
        board.generateAllMoves(boardsToEvalutate, Piece::TEAM::WHITE);

        std::unordered_map<int, int> activeWorkersToBoardIndex;
        
        int nextBoardToEvaluateIndex = boardsToEvalutate.size() - 1;
        for(size_t i = 1; i < worldSize && i < boardsToEvalutate.size(); i++) {
            activeWorkersToBoardIndex.insert(std::make_pair(i, nextBoardToEvaluateIndex));

            MPI_Send(boardsToEvalutate.at(nextBoardToEvaluateIndex).pieces, 65, MPI_INT, i, 0, MPI_COMM_WORLD);
            nextBoardToEvaluateIndex--;
        }

        MPI_Status status;
        std::vector<std::pair<Board, int>> results;
        while(activeWorkersToBoardIndex.size() != 0) {
            Board b;
            MPI_Recv(b.pieces, 65, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            results.push_back(std::make_pair(b, activeWorkersToBoardIndex.find(status.MPI_SOURCE)->second));

            if(nextBoardToEvaluateIndex < 0) {
                MPI_Send(b.pieces, 65, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                activeWorkersToBoardIndex.erase(status.MPI_SOURCE);
            } else {
                MPI_Send(boardsToEvalutate.at(nextBoardToEvaluateIndex).pieces, 65, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                activeWorkersToBoardIndex.find(status.MPI_SOURCE)->second = nextBoardToEvaluateIndex;
                nextBoardToEvaluateIndex--;
            }
        }

        size_t bestResultIndex = 0;
        for(size_t i = 1; i < results.size(); i++) {
            if(results.at(i).first.pieces[64] > results.at(bestResultIndex).first.pieces[64]) {
                bestResultIndex = i;
            }
        }

        boardsToEvalutate.at(results.at(bestResultIndex).second).print();
    } else {
        MPI_Status status;
        Board boardToEvaluate;

        MPI_Recv(boardToEvaluate.pieces, 65, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        while(status.MPI_TAG == 0) {
            Board evaluation = MiniMax::evaluateWhiteMove(boardToEvaluate, (2 * moveDepth) - 1);

            MPI_Send(evaluation.pieces, 65, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(boardToEvaluate.pieces, 65, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }

    MPI_Finalize();
}