#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.cpp"

using namespace std;

double add(double x) {
    return x + 1;
}

int main() {
    // Matrix m1(3, 3);
    // Matrix m2(3, 3);
    // Matrix m3(3, 3);

    // for(int i = 0; i < m1.getLength(); i++) {
    //     m1.getData()[i] = i;
    //     m2.getData()[i] = i;
    // }

    // m1.threadMultiply(&m2, &m3);

    // m1.print();

    // cout << endl;

    // m2.print();

    // cout << endl;

    // m3.print();

    Matrix m4(748, 16);
    Matrix m5(16, 748);
    Matrix m6(748, 748);

    clock_t start = clock();

    for(int i = 0; i < 10; i++) {
        m4.threadMultiply(&m5, &m6);
    }

    clock_t end = clock();

    double timeSpent = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%f seconds\n", timeSpent);
}