#include <cmath>

using namespace std;

class ActivationFunction {
    virtual double applyFunction(double x) = 0;
    virtual double applyDerivativeFunction(double x) = 0;
};