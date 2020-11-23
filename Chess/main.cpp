#include "Board.hpp"

int main() {
    Board b;
    b.setBoard();

    b.printDebug();

    std::cout << std::endl;

    b.print();
}