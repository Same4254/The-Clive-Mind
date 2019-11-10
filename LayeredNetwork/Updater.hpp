#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../Matrix/Matrix.hpp"

#endif

class Updater {
    public:
        virtual void update(Matrix* parameters, Matrix* gradient) = 0;
};