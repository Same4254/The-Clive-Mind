#pragma once

#include "Move.hpp"
#include "BoardState.hpp"

class PieceFunctionality {
protected:
    //The offsets for each move
    std::vector<std::pair<int, int>> moveOffsets;

    std::array<Score, PIECES::NUM_PIECES> pieceValues;

public:
    const PieceFunctionalityIndex piece;

    //The 0 or 1 from modding the piece by 2 to get the team
    const int teamModularity;

    //The character to draw on the screen for drawing the board in the terminal 
    const char renderCharacter;

    //Some pieces just don't move (empty spots on the board). So use this bool for an if to not bother with the piece
    const bool doesMove;

    /**
     * @param teamModularity -> Modularity of the team this piece is on. 0 for player 1, 1 for player 2
     * @param renderCharacter -> The character to draw when rendering this piece on the screen
     * @param doesMove -> States whether this piece can actually move. This is because empty slots on the board are "pieces" in this model
     */
    PieceFunctionality(const PieceFunctionalityIndex piece, const char renderCharacter, const bool doesMove);

    /**
     * The idea of this function is to perform a certain move on the board by filling in the Move object provided and calling the doMove function in the board functionality.
     * This move object will contain all info to do and undo the move.
     * 
     * In addtion, the return value of this function will be whether a move was performed
     * 
     * The piece will unconditionally make the move dictated by the provided moveIndex. 
     * It will not check if the move is legal, since the moveIndex will have been given by the previous fillMove call
     * The previous call will only provide the next move index if it is a legal move
     * This legality check can befaster when knowing previous indecies were legal
     *  - Unless the index is 0. This is becuase the first call does not know if there are any possible move at all
     *  - As such, check for the lack of any move in the move object when providing an index of 0 along with the return of -1
     * 
     * @param boardState -> The current state of the board to move on
     * @param move -> The move object to fill in with hash data, what pieces to move around, and other info to do AND undo a move
     * @param boardRow -> Row of the piece
     * @param boardCol -> Column of the piece
     * @param boardPositionIndex -> Location of the piece in 1D
     * @param moveIndex -> The index that represents which move for this piece to make
     * 
     * @return The index of the next move to make on this piece. If the result is -1, then there are no more moves to be made
     */
    virtual void fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const = 0;

    inline void takeSquare(BoardState &boardState, Move &move, int &amtMovesFilled, const PieceFunctionalityIndex newPiece, const BoardPieceIndex startRow, const BoardPieceIndex startCol, const BoardPieceIndex endRow, const BoardPieceIndex endCol) const {
        if(boardState.pieces2D[startRow][startCol] % 2 == boardState.pieces2D[endRow][endCol] % 2 && boardState.pieces2D[endRow][endCol] != PIECES::EMPTY)
            return;
        
        move.squaresChanged[move.amtSquaresChanged].row = startRow;
        move.squaresChanged[move.amtSquaresChanged].col = startCol;
        move.squaresChanged[move.amtSquaresChanged].oldPiece = piece;
        move.squaresChanged[move.amtSquaresChanged].newPiece = PIECES::EMPTY;

        move.amtSquaresChanged++;

        move.squaresChanged[move.amtSquaresChanged].row = endRow;
        move.squaresChanged[move.amtSquaresChanged].col = endCol;
        move.squaresChanged[move.amtSquaresChanged].oldPiece = boardState.pieces2D[endRow][endCol];
        move.squaresChanged[move.amtSquaresChanged].newPiece = newPiece;

        move.score -= pieceValues[piece];
        move.score -= pieceValues[boardState.pieces2D[endRow][endCol]];
        move.score += pieceValues[newPiece];

        amtMovesFilled++;
        move.amtSquaresChanged++;
    }
};