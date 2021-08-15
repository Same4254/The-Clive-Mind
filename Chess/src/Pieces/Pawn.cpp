#include "Pieces/Pawn.hpp"

Pawn::Pawn(const PieceFunctionalityIndex piece, const char renderCharacter) : PieceFunctionality(piece, renderCharacter, true) {
    whitePromotionPieces[0] = PIECES::WHITE_QUEEN;
    whitePromotionPieces[0] = PIECES::WHITE_KNIGHT;
    whitePromotionPieces[0] = PIECES::WHITE_ROOK;
    whitePromotionPieces[0] = PIECES::WHITE_BISHOP;

    blackPromotionPieces[0] = PIECES::BLACK_QUEEN;
    blackPromotionPieces[0] = PIECES::BLACK_KNIGHT;
    blackPromotionPieces[0] = PIECES::BLACK_ROOK;
    blackPromotionPieces[0] = PIECES::BLACK_BISHOP;

    if(teamModularity == TEAM::WHITE) {
        moveOffsets.push_back(std::make_pair(-1, 1));
        moveOffsets.push_back(std::make_pair(-1, -1));
        moveOffsets.push_back(std::make_pair(-1, 0));
    } else {
        moveOffsets.push_back(std::make_pair(1, 1));
        moveOffsets.push_back(std::make_pair(1, -1));
        moveOffsets.push_back(std::make_pair(1, 0));
    }
}

void Pawn::fillMoves(BoardState &boardState, std::array<Move, MAX_MOVES_PER_PIECE> &movesToFill, int &amtMovesFilled, const BoardPieceIndex boardRow, const BoardPieceIndex boardCol, const BoardPieceIndex boardPositionIndex) const {
    //First two are diagonal takes
    for(int i = 0; i <= 1; i++) {
        auto &p = moveOffsets.at(i);
        int nextRow = boardRow + p.first;
        int nextCol = boardCol + p.second;

        if(nextRow < 0 || nextRow >= BoardState::nRows || nextCol < 0 || nextCol >= BoardState::nCols)
            continue;

        if(boardState.pieces2D[nextRow][nextCol] != PIECES::EMPTY && boardState.pieces2D[nextRow][nextCol] % 2 != teamModularity) {
            if(nextRow == 0 || nextRow == 7) {
                //Promote
                for(int i = 0; i < whitePromotionPieces.size(); i++)
                    takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, teamModularity == TEAM::WHITE ? whitePromotionPieces[i] : blackPromotionPieces[i], boardRow, boardCol, nextRow, nextCol);
            } else {
                takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, teamModularity == TEAM::BLACK ? PIECES::BLACK_PAWN : PIECES::WHITE_PAWN, boardRow, boardCol, nextRow, nextCol);
            }
        }
    }

    auto &p = moveOffsets.at(2);

    int nextRow = boardRow + p.first;
    int nextCol = boardCol + p.second;

    if(boardState.pieces2D[nextRow][nextCol] == PIECES::EMPTY) {
        if(nextRow == 0 || nextRow == 7) {
            //Promote
            for(int i = 0; i < whitePromotionPieces.size(); i++)
                takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, teamModularity == TEAM::WHITE ? whitePromotionPieces[i] : blackPromotionPieces[i], boardRow, boardCol, nextRow, nextCol);
        } else {
            takeSquare(boardState, movesToFill[amtMovesFilled], amtMovesFilled, teamModularity == TEAM::BLACK ? PIECES::BLACK_PAWN : PIECES::WHITE_PAWN, boardRow, boardCol, nextRow, nextCol);
        }
    }
}