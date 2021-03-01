#include "Board.hpp"

int main() {
    Board b;
    b.setBoard();

    hash_type hash = b.hash;

    std::cout << hash << std::endl;
    b.printDebug();

    PieceFunctionality::getInstance().move(b, 1, 0, 2, 0);

    

    std::cout << b.hash << std::endl;
    b.printDebug();

    // std::cout << b.countMoves(true, 5) << std::endl;
}