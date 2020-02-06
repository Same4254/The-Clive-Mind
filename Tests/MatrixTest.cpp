#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.hpp"

using namespace std;

double add(double x) {
    return x + 1;
}

int main() {
    Matrix m1(11, 11);
    // for(int i = 0; i < m1.getLength(); i++) {
    //     m1.getData()[i] = i;
    // }

    Matrix m2(4, 4);
    for(int i = 0; i < m2.getLength(); i++) {
        m2.getData()[i] = i;
    }

    m1.print();

    printf("\n");

    m2.print();

    printf("\n");

    m2.dialatePad(&m1, 2, 2);

    m1.print();
}