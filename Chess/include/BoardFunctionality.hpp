#pragma once

#include <vector>
#include <memory>

#include "Pieces/PieceFunctionality.hpp"
#include "Pieces/EmptyPiece.hpp"
#include "Pieces/PawnDouble.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/BoardMovePieces/Bishop.hpp"
#include "Pieces/BoardMovePieces/Rook.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/King.hpp"

#include "BoardHashTable.hpp"

#include "BoardState.hpp"
#include "Types.hpp"

class BoardFunctionality {
private:
    BoardHashTable hashTable;

public:
    //Each element describes how a piece moves. The index corresponds to the values in the state
    std::array<std::unique_ptr<PieceFunctionality>, PIECES::NUM_PIECES> pieceFunctions;

    BoardFunctionality();

    void clearBoardState(BoardState &boardState) const;
    void setInitialBoardState(BoardState &BoardState) const;

    bool isSquareAttacked(BoardState &boardState, TEAM defendingTeam, int row, int col) const;

    void print(const BoardState &boardState) const;

    /**
     * Given the board state and the move, modify the board state to enact that move
     * 
     * Implementation in header to get inlining
     * 
     * @param boardState -> The board state to perform this move on
     * @param move -> The move to perform
     */
    inline void doMove(BoardState &boardState, const Move &move) const {
        for(uint32_t i = 0; i < move.amtSquaresChanged; i++) {
            const SquareInfo &info = move.squaresChanged[i];

            boardState.hash ^= hashTable.getHash(info.oldPiece, info.row, info.col);
            boardState.hash ^= hashTable.getHash(info.newPiece, info.row, info.col);

            boardState.pieces2D[info.row][info.col] = info.newPiece;
        }

        boardState.player1Turn = !boardState.player1Turn;
    }

    /**
     * Given the board state and the move, modify the board state to undo that move
     * This of course assumes that the move has been done. 
     * 
     * Implementation in header to get inlining
     * 
     * @param boardState -> The board state to undo this move upon
     * @param move -> The move to undo
     */
    inline void undoMove(BoardState &boardState, const Move &move) const {
        for(uint32_t i = 0; i < move.amtSquaresChanged; i++) {
            const SquareInfo &info = move.squaresChanged[i];

            boardState.hash ^= hashTable.getHash(info.newPiece, info.row, info.col);
            boardState.hash ^= hashTable.getHash(info.oldPiece, info.row, info.col);

            boardState.pieces2D[info.row][info.col] = info.oldPiece;
        }

        boardState.player1Turn = !boardState.player1Turn;
    }
};