#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.hpp"

using namespace std;

int main() {
    int N = 5;

    Matrix m(1, N);
    Matrix m2(N, N);

    for(int i = 0 ; i < N; i++) {
        m.getData()[i] = i;
    }

    m.print();

    // for(int i = 0; )

    // Matrix m(100, 100, -1, 1);
    // Matrix m2(100, 100, -1, 1);
    // Matrix m3(100, 100);

    // for(int i = 0; i < 1000; i++) {
    //     m.multiply(&m2, &m3);
    // }

    // m3.print();
}