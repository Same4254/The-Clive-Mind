#include <stdio.h>
#include <iostream>
#include <time.h>

#include "Matrix.cpp"

using namespace std;

double add(double x) {
    return x + 1;
}

int main() {
    Matrix m1(5, 3);
    // Matrix m2(3, 3);

    for(int i = 0; i < m1.getLength(); i++) {
        m1.getData()[i] = i;
        // m2.getData()[i] = i;
    }

    m1.mTranspose();

    m1.print();

    // cout << endl;

    // m2.print();

    m1.set(2, 4, 188);
    printf("%f\n", m1.at(2, 4));

    m1.print();
}