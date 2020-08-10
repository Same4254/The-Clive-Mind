#include "Environments/GridEnvironment.hpp"

GridEnvironment::GridEnvironment(int rows, int cols) {
    board = new Matrix(rows, cols, WALK_SQUARE);
    originalBoard = new Matrix(rows, cols, WALK_SQUARE);

    playerRow = 0;
    playerCol = 0;

    // originalBoard->set(1, 0, TERMINATE_SQUARE);
    // originalBoard->set(1, 1, TERMINATE_SQUARE);
    // originalBoard->set(1, 3, TERMINATE_SQUARE);

    // originalBoard->set(2, 3, TERMINATE_SQUARE);

    // originalBoard->set(3, 2, TERMINATE_SQUARE);
    // originalBoard->set(3, 3, TERMINATE_SQUARE);
    // originalBoard->set(3, 4, TERMINATE_SQUARE);
    // originalBoard->set(3, 5, TERMINATE_SQUARE);

    for(int i = 0; i < originalBoard->getLength(); i++) {
        if(originalBoard->getData()[i] != TERMINATE_SQUARE)
            emptySquares.push_back(i);
    }

    reset();
}

GridEnvironment::~GridEnvironment() {
    delete board;
}

void GridEnvironment::reset() {
    for(int i = 0; i < board->getLength(); i++) 
        board->getData()[i] = originalBoard->getData()[i];

    int vectorIndex = rand() % emptySquares.size();
    int playerIndex = emptySquares.at(vectorIndex);

    emptySquares.erase(emptySquares.begin() + vectorIndex);

    int goalIndex = emptySquares.at(rand() % emptySquares.size());

    emptySquares.push_back(playerIndex);

    board->getData()[playerIndex] = PLAYER_SQUARE;
    board->getData()[goalIndex] = GOAL_SQUARE;

    playerRow = playerIndex / board->getNCols();
    playerCol = playerIndex % board->getNCols();

    goalRow = goalIndex / board->getNCols();
    goalCol = goalIndex % board->getNCols();
}

Matrix* GridEnvironment::getBoard() {
    return board;
}

Matrix* GridEnvironment::getOriginalBoard() {
    return originalBoard;
}

/******* CAN MOVE *******/
bool GridEnvironment::canMove(int rowDir, int colDir) {
    return playerRow + rowDir >= 0 && playerRow + rowDir < board->getNRows() && 
            playerCol + colDir >= 0 && playerCol + colDir < board->getNCols();
}

bool GridEnvironment::canMoveUp() { return canMove(-1, 0); }
bool GridEnvironment::canMoveDown() { return canMove(1, 0); }
bool GridEnvironment::canMoveLeft() { return canMove(0, -1); }
bool GridEnvironment::canMoveRight() { return canMove(0, 1); }

bool GridEnvironment::canMoveByDirectionIndex(int index) { 
    if(index == 0)
        return canMoveUp();
    else if(index == 1)
        return canMoveDown();
    else if(index == 2)
        return canMoveLeft();
    else if(index == 3)
        return canMoveRight();
    
    std::cout << "Error: invalid movement index!" << std::endl;

    return false;
}

/******* MOVE *******/
double GridEnvironment::move(int rowDir, int colDir) {
    if(!canMove(rowDir, colDir)) return 0;

    // if(originalBoard->at(playerRow, playerCol) > 0)
        board->set(playerRow, playerCol, WALK_SQUARE);
    // else
    //     board->set(playerRow, playerCol, originalBoard->at(playerRow, playerCol));

    playerRow += rowDir;
    playerCol += colDir;

    double reward; 
    if(board->at(playerRow, playerCol) == TERMINATE_SQUARE)
        reward = TERMINATE_PUNISHMENT;
    else if(board->at(playerRow, playerCol) == WALK_SQUARE)
        reward = WALK_PUNISHMENT;
        // reward = 1.0 / sqrt(pow(playerRow - goalRow, 2) + pow(playerCol - goalCol, 2));
    else //if(board->at(playerRow, playerCol) == GOAL_SQUARE)
        reward = GOAL_REWARD;

    board->set(playerRow, playerCol, PLAYER_SQUARE);

    return reward;
}

double GridEnvironment::moveUp() { return move(-1, 0); }
double GridEnvironment::moveDown() { return move(1, 0); }
double GridEnvironment::moveLeft() { return move(0, -1); }
double GridEnvironment::moveRight() { return move(0, 1); }

double GridEnvironment::moveByDirectionIndex(int index) { 
    if(index == 0)
        return moveUp();
    else if(index == 1)
        return moveDown();
    else if(index == 2)
        return moveLeft();
    else if(index == 3)
        return moveRight();
    
    std::cout << "Error: invalid movement index!" << std::endl;

    return 0.0;
}

int GridEnvironment::getPlayerRow() { return playerRow; }
int GridEnvironment::getPlayerCol() { return playerCol; }