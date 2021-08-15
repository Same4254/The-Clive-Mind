#include "Solvers/MiniMaxSolver.hpp"

Move MinimaxSolver::solve(BoardState &boardState, const BoardFunctionality &boardFunctionality, uint32_t depth) {
    Move bestMove;
    BoardPieceIndex index = 0;

    //The piece will fill this variable in to tell how many moves it generated
    int numMoves = 0;

    //Accept the first legal move without comparison
    bool anyMove = true;

    TEAM movingTeam = boardState.player1Turn ? TEAM::WHITE : TEAM::BLACK;

    PieceFunctionalityIndex king = boardState.player1Turn ? PIECES::WHITE_KING : PIECES::BLACK_KING;
    PieceFunctionalityIndex kingCastle = boardState.player1Turn ? PIECES::WHITE_KING_CASTLE_ABLE : PIECES::BLACK_KING_CASTLE_ABLE;

    std::array<Move, MAX_MOVES_PER_PIECE> pieceMoves;

    for(BoardPieceIndex row = 0; row < BoardState::nRows; row++) {
    for(BoardPieceIndex col = 0; col < BoardState::nCols; col++) {
        if(!boardFunctionality.pieceFunctions[boardState.pieces[index]]->doesMove || boardState.pieces2D[row][col] % 2 != movingTeam) {
            index++;
            continue;
        }

        // std::cout << "Depth: " << depth << ", Piece: " << boardState.pieces2D[row][col] << std::endl;

        //Reset moves to initial state
        for(auto &m : pieceMoves) {
            m.amtSquaresChanged = 0;
            m.score = 0;
            m.depth = depth;
        }

        numMoves = 0;

        //Fills pieceMoves with all of the moves the piece can do, and numMoves is how many moves to look through
        boardFunctionality.pieceFunctions[boardState.pieces[index]]->fillMoves(boardState, pieceMoves, numMoves, row, col, index);

        for(int i = 0; i < numMoves; i++) {
            //Make a copy because the recursive call will likely overwrite what is in the array
            Move tempMove = pieceMoves[i];

            //Check if move results in King check, reject move if it is
            boardFunctionality.doMove(boardState, tempMove);

            bool reject = false;

            for(int r = 0; r < BoardState::nRows; r++) {
            for(int c = 0; c < BoardState::nCols; c++) {
                if(boardState.pieces2D[r][c] == king || boardState.pieces2D[r][c] == kingCastle) {
                    if(boardFunctionality.isSquareAttacked(boardState, movingTeam, r, c)) {
                        reject = true;
                    }
                }
            }}

            if(reject) {
                boardFunctionality.undoMove(boardState, tempMove);
                continue;
            }

            //No need to perform the move if there is no deeper to go than this move
            if(depth - 1 != 0) {
                // std::cout << "Going deeper on: " << depth << std::endl;
                // boardFunctionality.print(boardState);

                //Find the best score to achieve with this move
                Move bestResult = solve(boardState, boardFunctionality, depth - 1);

                // std::cout << "Returned to: " << boardState.player1Turn << std::endl;
                // boardFunctionality.print(boardState);

                //Get the total score, and the depth it requires to get that score
                tempMove.score += bestResult.score;
                tempMove.depth = bestResult.depth;//How far down you have to go to get this score
            }

            //Set the board back to what it originally was
            boardFunctionality.undoMove(boardState, tempMove);

            // std::cout << "Undone: " << boardState.player1Turn << std::endl;
            // boardFunctionality.print(boardState);

            if(anyMove || (boardState.player1Turn && bestMove < tempMove) || (!boardState.player1Turn && !(bestMove < tempMove))) {
                anyMove = false;
                bestMove = tempMove;//POD classes coming in handy with that super fast copy
            }
        }

        index++;
    }}

    //No move was chosen, it is either a mate or draw
    if(anyMove) {
        bestMove.depth = depth;

        //Look for check
        bool inCheck = false;

        for(int row = 0; row < BoardState::nRows; row++) {
        for(int col = 0; col < BoardState::nCols; col++) {
            if(boardState.pieces2D[row][col] == king || boardState.pieces2D[row][col] == kingCastle) {
                if(boardFunctionality.isSquareAttacked(boardState, boardState.player1Turn ? TEAM::WHITE : TEAM::BLACK, row, col)) {
                    inCheck = true;
                }
            }
        }}

        bestMove.score = inCheck ? (boardState.player1Turn ? -100 : 100) : 0;
    }

    return bestMove;
}