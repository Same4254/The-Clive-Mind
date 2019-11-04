#include "../Matrix/Matrix.hpp"

int main() { 
    Matrix planet1Pos(3, 1);
    planet1Pos.set(0, 0, 1);

    Matrix planet2Pos(3, 1);
    Matrix planet3Pos(3, 1);

    Matrix planet1Vel(3, 1);
    planet1Vel.set(1, 0, 1);

    Matrix planet2Vel(3, 1);
    Matrix planet3Vel(3, 1);

    Matrix planet1Acc(3, 1);
    Matrix planet2Acc(3, 1);
    Matrix planet3Acc(3, 1);

    Matrix oneTwoPosDifference(3, 1);
    Matrix oneThreePosDifference(3, 1);
    Matrix twoThreePosDifference(3, 1);

    Matrix temp(3, 1);

    double t = 0.0;
    double dt = 0.0001;

    double g = -1.0;
    double m1 = 1.0;
    double m2 = 5.0;
    double m3 = 5.0;

    for(int i = 0; i < 3000; i++) {
        planet1Pos.subtract(&planet2Pos, &oneTwoPosDifference);
        planet1Pos.subtract(&planet3Pos, &oneThreePosDifference);
        planet2Pos.subtract(&planet3Pos, &twoThreePosDifference);

        oneTwoPosDifference.scale((g * m2) / pow(oneTwoPosDifference.vectorMagnitude(), 3), &planet1Acc);
        oneThreePosDifference.scale((g * m3) / pow(oneThreePosDifference.vectorMagnitude(), 3), &temp);
        planet1Acc.mAdd(&temp);

        planet1Acc.scale(dt, &temp);
        planet1Vel.mAdd(&temp);
        
        planet1Vel.scale(dt * .5, &temp);
        planet1Pos.mAdd(&temp);

        t += dt;

        planet1Pos.print();
        printf("-----\n");
    }
}