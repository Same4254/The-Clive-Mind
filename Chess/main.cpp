#include "BoardState.hpp"
#include "BoardFunctionality.hpp"
#include "Solvers/MiniMaxSolver.hpp"

void printAllMoves(BoardState &boardState, BoardFunctionality &boardFunctionality, TEAM team) {
    std::cout << "Start:" << std::endl;

    boardFunctionality.print(boardState);

    if(team == TEAM::WHITE)
        std::cout << "White moves:" << std::endl;
    else
        std::cout << "Black moves:" << std::endl;

    std::array<Move, MAX_MOVES_PER_PIECE> pieceMoves;

    int numMoves = 0;
    int index = 0;

    for(BoardPieceIndex row = 0; row < BoardState::nRows; row++) {
    for(BoardPieceIndex col = 0; col < BoardState::nCols; col++) {
        if(!boardFunctionality.pieceFunctions[boardState.pieces[index]]->doesMove || boardState.pieces[index] % 2 != team) {
            index++;
            continue;
        }

        //Reset moves to initial state
        for(auto &m : pieceMoves) {
            m.amtSquaresChanged = 0;
            m.score = 0;
        }

        numMoves = 0;

        //Fills pieceMoves with all of the moves the piece can do, and numMoves is how many moves to look through
        boardFunctionality.pieceFunctions[boardState.pieces[index]]->fillMoves(boardState, pieceMoves, numMoves, row, col, index);

        for(int i = 0; i < numMoves; i++) {
            Move &move = pieceMoves[i];

            boardFunctionality.doMove(boardState, move);

            boardFunctionality.print(boardState);
            
            boardFunctionality.undoMove(boardState, move);
        }
        

        index++;
    }}
}

void printAllMoves(BoardState &boardState, BoardFunctionality &boardFunctionality) {
    printAllMoves(boardState, boardFunctionality, TEAM::WHITE);
    printAllMoves(boardState, boardFunctionality, TEAM::BLACK);
}

void testPawns() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    for(int col = 0; col < 8; col++)
        boardState.pieces2D[6][col] = PIECES::WHITE_PAWN_DOUBLE; 

    // for(int col = 0; col < 8; col++)
    //     boardState.pieces2D[5][col] = PIECES::BLACK_PAWN; 

    printAllMoves(boardState, boardFunctionality);
}

void testKnight() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[0][0] = PIECES::BLACK_KNIGHT;

    // boardState.pieces2D[2][1] = PIECES::BLACK_PAWN;

    printAllMoves(boardState, boardFunctionality);
}

void testBishop() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[0][0] = PIECES::BLACK_BISHOP;

    // boardState.pieces2D[2][1] = PIECES::BLACK_PAWN;

    printAllMoves(boardState, boardFunctionality);
}

void testRook() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[3][3] = PIECES::BLACK_ROOK;

    // boardState.pieces2D[2][1] = PIECES::BLACK_PAWN;

    printAllMoves(boardState, boardFunctionality);
}

void testQueen() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[3][3] = PIECES::BLACK_QUEEN;

    // boardState.pieces2D[2][1] = PIECES::BLACK_PAWN;

    printAllMoves(boardState, boardFunctionality);
}

void testKing() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[3][3] = PIECES::BLACK_KING;

    // boardState.pieces2D[2][1] = PIECES::BLACK_PAWN;

    printAllMoves(boardState, boardFunctionality);
}

void testFirstMove() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    boardFunctionality.setInitialBoardState(boardState);

    printAllMoves(boardState, boardFunctionality);
}

void testAttackedSquare() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;

    for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}

    boardState.pieces2D[3][3] = PIECES::BLACK_PAWN;

    std::cout << "Attacked Squares: " <<std::endl;

    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            if(row == 3 && col == 3) {
                std::cout << "  ";
                continue;
            }

            std::cout << (boardFunctionality.isSquareAttacked(boardState, TEAM::WHITE, row, col) ? "T " : "F ");
        }

        std::cout << std::endl;
    }
}

void testMateIn1() {
    BoardState boardState;
    boardState.set2DPointers();

    BoardFunctionality boardFunctionality;
    boardFunctionality.clearBoardState(boardState);

    boardState.pieces2D[0][6] = PIECES::BLACK_KING;
    boardState.pieces2D[1][5] = PIECES::BLACK_PAWN_DOUBLE;
    boardState.pieces2D[1][6] = PIECES::BLACK_PAWN_DOUBLE;
    boardState.pieces2D[1][7] = PIECES::BLACK_PAWN_DOUBLE;
    // boardState.pieces2D[3][7] = PIECES::BLACK_PAWN;

    boardState.pieces2D[7][7] = PIECES::WHITE_KING;
    boardState.pieces2D[7][4] = PIECES::WHITE_QUEEN;
    boardState.player1Turn = true;

    // boardState.pieces2D[0][4] = PIECES::WHITE_QUEEN;
    // boardState.player1Turn = false;

    std::cout << "Original: " << std::endl;

    boardFunctionality.print(boardState);

    std::cout << "---------------" << std::endl;

    MinimaxSolver solver;
    Move bestMove = solver.solve(boardState, boardFunctionality, 4);

    std::cout << "Best Move Score: " << bestMove.score << std::endl;

    boardFunctionality.doMove(boardState, bestMove);

    boardFunctionality.print(boardState);

    // std::cout << boardFunctionality.isSquareAttacked(boardState, TEAM::BLACK, 0, 6) << std::endl;
}

int main() {
    std::cout << std::is_pod<Move>() << std::endl;

    // testPawns();
    // testKnight();

    // testBishop();
    // testRook();

    // testQueen();
    // testKing();

    // testFirstMove();

    // testAttackedSquare();
    testMateIn1();
}