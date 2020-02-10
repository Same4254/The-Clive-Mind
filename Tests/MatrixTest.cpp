#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.hpp"

using namespace std;

int main() {
    Matrix m(100, 100, -1, 1);
    Matrix m2(100, 100, -1, 1);
    Matrix m3(100, 100);

    for(int i = 0; i < 1000; i++) {
        m.multiply(&m2, &m3);
    }

    m3.print();
}