#ifndef GRID_ENVIRONMENT_HPP
#define GRID_ENVIRONMENT_HPP

#include "Matrix.hpp"
#include <vector>

#define TERMINATE_SQUARE -1.0
#define GOAL_SQUARE 1.0
#define PLAYER_SQUARE 0.5
#define WALK_SQUARE 0.0

#define WALK_PUNISHMENT -0.01
#define TERMINATE_PUNISHMENT -1.0
#define GOAL_REWARD 1.0

class GridEnvironment {
private:
    Matrix* board;
    Matrix* originalBoard;

    std::vector<int> emptySquares;

    int playerRow, playerCol;
    int goalRow, goalCol;

    bool canMove(int rowDir, int colDir);
    double move(int rowDir, int colDir);

public:
    GridEnvironment(int rows, int columns);
    ~GridEnvironment();

    void reset();

    Matrix* getBoard();
    Matrix* getOriginalBoard();

    bool canMoveUp();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();
    bool canMoveByDirectionIndex(int index);

    double moveUp();
    double moveDown();
    double moveLeft();
    double moveRight();
    double moveByDirectionIndex(int index);

    int getPlayerRow();
    int getPlayerCol();
};

#endif