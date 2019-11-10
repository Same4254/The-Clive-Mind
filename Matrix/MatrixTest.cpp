#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.hpp"

using namespace std;

double add(double x) {
    return x + 1;
}

int main() {
    Matrix m1(3, 3);
    for(int i = 0; i < m1.getLength(); i++) {
        m1.getData()[i] = i;
    }

    Matrix m2(15, 15);

    m1.print();

    printf("\n");

    m1.dialate(&m2, 4);

    m2.print();
}