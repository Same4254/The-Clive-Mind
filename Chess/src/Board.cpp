#include "Board.hpp"

const int Board::STATE_LENGTH = 64;

Board::Board() {
    //Chess board is 8x8 board. Thus, 64 indecies long for the board. In addition, one more for the score
    state = (PieceIndexType*) calloc(STATE_LENGTH, sizeof(PieceIndexType));
    pieces2D = (PieceIndexType**) malloc(sizeof(PieceIndexType*) * 8);

    //Set the row pointer to the pointer at the location in the 1D allocation
    for(int i = 0; i < 8; i++)
        pieces2D[i] = &state[i * 8];

    //initialize random generation with seed
    std::mt19937_64 r;
    r.seed(1729);

    std::uniform_int_distribution<hash_type> distribution;

    for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
    for(int p = 1; p < 21; p++) {// starts at one so that all of the empty piece hashes are 0
        pieceToHash[i][j][p] = distribution(r);
    }}}

    // Set empty piece hashes to 0
    for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
        pieceToHash[i][j][0] = 0;
    }}

    hash = 0;
}

Board::~Board() {
    //Be freeeee!!!!
    free(state);
    free(pieces2D);
}

void Board::setBoard() {
    pieces2D[0][0] = Piece::TYPE::BLACK_ROOK_CASTLE_ABLE;
    pieces2D[0][1] = Piece::TYPE::BLACK_KNIGHT;
    pieces2D[0][2] = Piece::TYPE::BLACK_BISHOP;
    pieces2D[0][3] = Piece::TYPE::BLACK_QUEEN;
    pieces2D[0][4] = Piece::TYPE::BLACK_KING_CASTLE_ABLE;
    pieces2D[0][5] = Piece::TYPE::BLACK_BISHOP;
    pieces2D[0][6] = Piece::TYPE::BLACK_KNIGHT;
    pieces2D[0][7] = Piece::TYPE::BLACK_ROOK_CASTLE_ABLE;

    for(int i = 0; i <= 7; i++)
        pieces2D[1][i] = Piece::TYPE::BLACK_PAWN_DOUBLE;

    for(int i = 2; i <= 5; i++) {
        for(int j = 0; j <= 7; j++) {
            pieces2D[i][j] = Piece::TYPE::EMPTY;
        }
    }

    for(int i = 0; i <= 7; i++)
        pieces2D[6][i] = Piece::TYPE::WHITE_PAWN_DOUBLE;

    pieces2D[7][0] = Piece::TYPE::WHITE_ROOK_CASTLE_ABLE;
    pieces2D[7][1] = Piece::TYPE::WHITE_KNIGHT;
    pieces2D[7][2] = Piece::TYPE::WHITE_BISHOP;
    pieces2D[7][3] = Piece::TYPE::WHITE_QUEEN;
    pieces2D[7][4] = Piece::TYPE::WHITE_KING_CASTLE_ABLE;
    pieces2D[7][5] = Piece::TYPE::WHITE_BISHOP;
    pieces2D[7][6] = Piece::TYPE::WHITE_KNIGHT;
    pieces2D[7][7] = Piece::TYPE::WHITE_ROOK_CASTLE_ABLE;

    hash = generateHash();
}

hash_type Board::generateHash() {
    hash_type toRet = 0;

    for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
        toRet ^= pieceToHash[i][j][pieces2D[i][j]];
    }}

    return toRet;
}

int Board::countMoves(bool whiteToMove, int plys) {
    std::unordered_set<hash_type> visitedPositions;

    return countMovesHelper(visitedPositions, whiteToMove, plys);
}

int Board::countMovesHelper(std::unordered_set<hash_type> &visitedPositions, bool whiteToMove, int plys) {
    if(plys == 0)//No more moves to try
        return 1;

    int moveSum = 0;

    //DFS on every non empty piece on the board
    for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
        if(pieces2D[i][j] == 0)
            continue;

        Piece *piece = PieceFunctionality::getInstance().pieceFunctions[pieces2D[i][j]];

        //Only move the given team's pieces
        if((whiteToMove && pieces2D[i][j] % 2 != 0) || (!whiteToMove && pieces2D[i][j] % 2 == 0))
            continue;

        for(size_t m = 0; m < piece->rowMovementOffsets.size(); m++) {
            int endRow = i + piece->rowMovementOffsets[m];
            int endColumn = j + piece->columnMovementOffsets[m];

            if(PieceFunctionality::getInstance().canMove(pieces2D, i, j, endRow, endColumn)) {
                //Get the piece that we are moving onto, in order for it to be restored later for the next branch
                PieceIndexType pieceReplaced = pieces2D[endRow][endColumn];
                PieceIndexType pieceMoved = pieces2D[i][j];

                PieceFunctionality::getInstance().move(*this, i, j, i + piece->rowMovementOffsets[m], j + piece->columnMovementOffsets[m]);

                // hash ^= pieceToHash[i][j][pieceMoved] ^ pieceToHash[endRow][endColumn][pieceReplaced] ^ pieceToHash[endRow][endColumn][pieces2D[endRow][endColumn]];
                
                hash_type hash = generateHash();

                std::cout << hash << std::endl;
                
                //Check to see if we have tried this position already
                if(visitedPositions.find(hash) == visitedPositions.end()) {
                    //Account for this move
                    visitedPositions.insert(hash);

                    //DFS
                    moveSum += countMovesHelper(visitedPositions, !whiteToMove, plys - 1);
                }

                //Restore the position
                pieces2D[i][j] = pieceMoved;
                pieces2D[i + piece->rowMovementOffsets[m]][j + piece->columnMovementOffsets[m]] = pieceReplaced;
            }
        }
    }}

    return moveSum;
}

void Board::clear() {
    for(int i = 0; i <= 7; i++) {
        for(int j = 0; j <= 7; j++) {
            pieces2D[i][j] = Piece::TYPE::EMPTY;
        }
    }
}

void Board::print() {
    for(int row = 0; row < 8; row++) {
        std::cout << 8 - row << " | ";
        for(int column = 0; column < 8; column++) {
            std::cout << PieceFunctionality::getInstance().getDisplayCharacter(pieces2D[row][column]) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "   ---------------- Score: " << (int) state[64] << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
}

void Board::printDebug() {
    for(int row = 0; row < 8; row++) {
        std::cout << 8 - row << " | ";
        for(int column = 0; column < 8; column++) {
            std::cout << std::setfill('0') << std::setw(2) << (int) pieces2D[row][column] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "   ------------------------" << std::endl;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
}