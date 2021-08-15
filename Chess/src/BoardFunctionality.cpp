#include "BoardFunctionality.hpp"

BoardFunctionality::BoardFunctionality() : hashTable(pieceFunctions.size(), BoardState::nRows, BoardState::nCols) {
    pieceFunctions[PIECES::EMPTY] = std::make_unique<EmptyPiece>('.');

    pieceFunctions[PIECES::WHITE_PAWN] = std::make_unique<Pawn>(PIECES::WHITE_PAWN, '1');
    pieceFunctions[PIECES::BLACK_PAWN] = std::make_unique<Pawn>(PIECES::BLACK_PAWN, '2');
    pieceFunctions[PIECES::WHITE_PAWN_DOUBLE] = std::make_unique<PawnDouble>(PIECES::WHITE_PAWN_DOUBLE, 'D');
    pieceFunctions[PIECES::BLACK_PAWN_DOUBLE] = std::make_unique<PawnDouble>(PIECES::BLACK_PAWN_DOUBLE, 'd');

    pieceFunctions[PIECES::BLACK_KNIGHT] = std::make_unique<Knight>(PIECES::BLACK_KNIGHT, 'n');
    pieceFunctions[PIECES::WHITE_KNIGHT] = std::make_unique<Knight>(PIECES::WHITE_KNIGHT, 'N');

    pieceFunctions[PIECES::BLACK_BISHOP] = std::make_unique<Bishop>(PIECES::BLACK_BISHOP, 'f');
    pieceFunctions[PIECES::WHITE_BISHOP] = std::make_unique<Bishop>(PIECES::WHITE_BISHOP, 'F');

    pieceFunctions[PIECES::WHITE_ROOK] = std::make_unique<Rook>(PIECES::WHITE_ROOK, 'R');
    pieceFunctions[PIECES::BLACK_ROOK] = std::make_unique<Rook>(PIECES::BLACK_ROOK, 'r');

    pieceFunctions[PIECES::WHITE_ROOK_CASTLE_ABLE] = std::make_unique<Rook>(PIECES::WHITE_ROOK_CASTLE_ABLE, '[');
    pieceFunctions[PIECES::BLACK_ROOK_CASTLE_ABLE] = std::make_unique<Rook>(PIECES::BLACK_ROOK_CASTLE_ABLE, '>');

    pieceFunctions[PIECES::WHITE_QUEEN] = std::make_unique<Queen>(PIECES::WHITE_QUEEN, '=');
    pieceFunctions[PIECES::BLACK_QUEEN] = std::make_unique<Queen>(PIECES::BLACK_QUEEN, '~');

    pieceFunctions[PIECES::WHITE_KING] = std::make_unique<King>(PIECES::WHITE_KING, '/');
    pieceFunctions[PIECES::BLACK_KING] = std::make_unique<King>(PIECES::BLACK_KING, '\\');
}

void BoardFunctionality::clearBoardState(BoardState &boardState) const {
    for(uint32_t row = 0; row < boardState.nRows; row++) {
    for(uint32_t col = 0; col < boardState.nCols; col++) {
        boardState.pieces2D[row][col] = PIECES::EMPTY;
    }}
}

void BoardFunctionality::setInitialBoardState(BoardState &boardState) const {
    clearBoardState(boardState);

    for(uint32_t col = 0; col < boardState.nCols; col++)
        boardState.pieces2D[6][col] = PIECES::WHITE_PAWN_DOUBLE;
    
    boardState.pieces2D[7][0] = PIECES::WHITE_ROOK_CASTLE_ABLE;
    boardState.pieces2D[7][1] = PIECES::WHITE_KNIGHT;
    boardState.pieces2D[7][2] = PIECES::WHITE_BISHOP;
    boardState.pieces2D[7][3] = PIECES::WHITE_QUEEN;
    boardState.pieces2D[7][4] = PIECES::WHITE_KING;
    boardState.pieces2D[7][5] = PIECES::WHITE_BISHOP;
    boardState.pieces2D[7][6] = PIECES::WHITE_KNIGHT;
    boardState.pieces2D[7][7] = PIECES::WHITE_ROOK_CASTLE_ABLE;

    for(uint32_t col = 0; col < boardState.nCols; col++)
        boardState.pieces2D[1][col] = PIECES::BLACK_PAWN_DOUBLE;
    
    boardState.pieces2D[0][0] = PIECES::BLACK_ROOK_CASTLE_ABLE;
    boardState.pieces2D[0][1] = PIECES::BLACK_KNIGHT;
    boardState.pieces2D[0][2] = PIECES::BLACK_BISHOP;
    boardState.pieces2D[0][3] = PIECES::BLACK_QUEEN;
    boardState.pieces2D[0][4] = PIECES::BLACK_KING;
    boardState.pieces2D[0][5] = PIECES::BLACK_BISHOP;
    boardState.pieces2D[0][6] = PIECES::BLACK_KNIGHT;
    boardState.pieces2D[0][7] = PIECES::BLACK_ROOK_CASTLE_ABLE;
}

bool BoardFunctionality::isSquareAttacked(BoardState &boardState, TEAM defendingTeam, int row, int col) const {
    if(defendingTeam == TEAM::WHITE) {
        if(col < 7 && row > 0 && (boardState.pieces2D[row - 1][col + 1] == PIECES::BLACK_PAWN ||
                                boardState.pieces2D[row - 1][col + 1] == PIECES::BLACK_PAWN_DOUBLE)) {
            return true;
        }

        if(col > 0 && row > 0 && (boardState.pieces2D[row - 1][col - 1] == PIECES::BLACK_PAWN ||
                                boardState.pieces2D[row - 1][col - 1] == PIECES::BLACK_PAWN_DOUBLE)) {
            return true;
        }
    }

    if(defendingTeam == TEAM::BLACK) {
        if(col < 7 && row > 0 && (boardState.pieces2D[row + 1][col + 1] == PIECES::WHITE_PAWN ||
                                boardState.pieces2D[row + 1][col + 1] == PIECES::WHITE_PAWN_DOUBLE)) {
            return true;
        }

        if(col > 0 && row > 0 && (boardState.pieces2D[row + 1][col - 1] == PIECES::WHITE_PAWN ||
                                boardState.pieces2D[row + 1][col - 1] == PIECES::WHITE_PAWN_DOUBLE)) {
            return true;
        }
    }

    PieceFunctionalityIndex enemyRook = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_ROOK : PIECES::BLACK_ROOK);
    PieceFunctionalityIndex enemyCastleRook = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_ROOK_CASTLE_ABLE : PIECES::BLACK_ROOK_CASTLE_ABLE);
    PieceFunctionalityIndex enemyQueen = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_QUEEN : PIECES::BLACK_QUEEN);

    PieceFunctionalityIndex enemyBishop = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_BISHOP : PIECES::BLACK_BISHOP);
    PieceFunctionalityIndex enemyKnight = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_KNIGHT : PIECES::BLACK_KNIGHT);

    PieceFunctionalityIndex enemyKing = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_KING : PIECES::BLACK_KING);
    PieceFunctionalityIndex enemyKingCastle = (defendingTeam == TEAM::BLACK ? PIECES::WHITE_KING_CASTLE_ABLE : PIECES::BLACK_KING_CASTLE_ABLE);

    for(int tempRow = row + 1; tempRow < 8; tempRow++) {
        if(boardState.pieces2D[tempRow][col] == PIECES::EMPTY)
            continue;

        if(boardState.pieces2D[tempRow][col] == enemyRook || boardState.pieces2D[tempRow][col] == enemyCastleRook || boardState.pieces2D[tempRow][col] == enemyQueen)
            return true;
        
        break;
    }

    for(int tempRow = row - 1; tempRow >= 0; tempRow--) {
        if(boardState.pieces2D[tempRow][col] == PIECES::EMPTY)
            continue;

        if(boardState.pieces2D[tempRow][col] == enemyRook || boardState.pieces2D[tempRow][col] == enemyCastleRook || boardState.pieces2D[tempRow][col] == enemyQueen)
            return true;
        
        break;
    }

    for(int tempColumn = col - 1; tempColumn >= 0; tempColumn--) {
        if(boardState.pieces2D[row][tempColumn] == PIECES::EMPTY)
            continue;

        if(boardState.pieces2D[row][tempColumn] == enemyRook || boardState.pieces2D[row][tempColumn] == enemyCastleRook || boardState.pieces2D[row][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    for(int tempColumn = col + 1; tempColumn < 8; tempColumn++) {
        if(boardState.pieces2D[row][tempColumn] == PIECES::EMPTY)
            continue;

        if(boardState.pieces2D[row][tempColumn] == enemyRook || boardState.pieces2D[row][tempColumn] == enemyCastleRook || boardState.pieces2D[row][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    int rowFactor = 1;
    int columnFactor = 1;

    int tempRow = row + rowFactor;
    int tempColumn = col + columnFactor;

    while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
        if(boardState.pieces2D[tempRow][tempColumn] == PIECES::EMPTY) {
            tempRow += rowFactor;
            tempColumn += columnFactor;

            continue;
        }

        if(boardState.pieces2D[tempRow][tempColumn] == enemyBishop || boardState.pieces2D[tempRow][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    rowFactor = -1;
    columnFactor = 1;

    tempRow = row + rowFactor;
    tempColumn = col + columnFactor;

    while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
        if(boardState.pieces2D[tempRow][tempColumn] == PIECES::EMPTY) {
            tempRow += rowFactor;
            tempColumn += columnFactor;

            continue;
        }

        if(boardState.pieces2D[tempRow][tempColumn] == enemyBishop || boardState.pieces2D[tempRow][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    rowFactor = 1;
    columnFactor = -1;

    tempRow = row + rowFactor;
    tempColumn = col + columnFactor;

    while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
        if(boardState.pieces2D[tempRow][tempColumn] == PIECES::EMPTY) {
            tempRow += rowFactor;
            tempColumn += columnFactor;

            continue;
        }

        if(boardState.pieces2D[tempRow][tempColumn] == enemyBishop || boardState.pieces2D[tempRow][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    rowFactor = -1;
    columnFactor = -1;

    tempRow = row + rowFactor;
    tempColumn = col + columnFactor;

    while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7) {
        if(boardState.pieces2D[tempRow][tempColumn] == PIECES::EMPTY) {
            tempRow += rowFactor;
            tempColumn += columnFactor;

            continue;
        }

        if(boardState.pieces2D[tempRow][tempColumn] == enemyBishop || boardState.pieces2D[tempRow][tempColumn] == enemyQueen)
            return true;
        
        break;
    }

    if(row - 2 >= 0 && col + 1 <= 7 && boardState.pieces2D[row - 2][col + 1] == enemyKnight)
        return true;
    
    if(row - 1 >= 0 && col + 2 <= 7 && boardState.pieces2D[row - 1][col + 2] == enemyKnight)
        return true;

    if(row - 2 >= 0 && col - 1 >= 0 && boardState.pieces2D[row - 2][col - 1] == enemyKnight)
        return true;
    
    if(row - 1 >= 0 && col - 2 >= 0 && boardState.pieces2D[row - 1][col - 2] == enemyKnight)
        return true;

    // ***

    if(row + 2 <= 7 && col - 1 >= 0 && boardState.pieces2D[row + 2][col - 1] == enemyKnight)
        return true;
    
    if(row + 1 <= 7 && col - 2 >= 0 && boardState.pieces2D[row + 1][col - 2] == enemyKnight)
        return true;

    if(row + 2 <= 7 && col + 1 <= 7 && boardState.pieces2D[row + 2][col + 1] == enemyKnight)
        return true;
    
    if(row + 1 <= 7 && col + 2 <= 7 && boardState.pieces2D[row + 1][col + 2] == enemyKnight)
        return true;

    //Check for enemy king
    if(row - 1 >= 0 && col - 1 >= 0 && (boardState.pieces2D[row - 1][col - 1] == enemyKing || boardState.pieces2D[row - 1][col - 1] == enemyKingCastle))
        return true;

    if(row - 1 >= 0 && (boardState.pieces2D[row - 1][col] == enemyKing || boardState.pieces2D[row - 1][col] == enemyKingCastle))
        return true;

    if(row - 1 >= 0 && col + 1 <= 7 && (boardState.pieces2D[row - 1][col + 1] == enemyKing || boardState.pieces2D[row - 1][col + 1] == enemyKingCastle))
        return true;

    if(col - 1 >= 0 && (boardState.pieces2D[row][col - 1] == enemyKing || boardState.pieces2D[row][col - 1] == enemyKingCastle))
        return true;

    if(col + 1 <= 7 && (boardState.pieces2D[row][col + 1] == enemyKing || boardState.pieces2D[row][col + 1] == enemyKingCastle))
        return true;

    if(row + 1 <= 7 && col - 1 >= 0 && (boardState.pieces2D[row + 1][col - 1] == enemyKing || boardState.pieces2D[row + 1][col - 1] == enemyKingCastle))
        return true;

    if(row + 1 <= 7 && (boardState.pieces2D[row + 1][col] == enemyKing || boardState.pieces2D[row + 1][col] == enemyKingCastle))
        return true;

    if(row + 1 <= 7 && col + 1 <= 7 && (boardState.pieces2D[row + 1][col + 1] == enemyKing || boardState.pieces2D[row + 1][col + 1] == enemyKingCastle))
        return true;

    return false;
}

void BoardFunctionality::print(const BoardState &boardState) const {
    for(uint32_t row = 0; row < boardState.nRows; row++) {
        std::cout << (boardState.nRows - row) << " | ";

        for(uint32_t col = 0; col < boardState.nCols; col++)
            std::cout << pieceFunctions[boardState.pieces2D[row][col]]->renderCharacter << " ";
        
        std::cout << std::endl;
    }

    std::cout << "   ";
    for(uint32_t col = 0; col < boardState.nCols * 2; col++)
        std::cout << "-";
    std::cout << std::endl;

    std::cout << "    ";
    for(uint32_t col = 0; col < boardState.nCols; col++)
        std::cout << (char)('a' + col) << " ";
    std::cout << std::endl;
}