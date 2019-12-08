#include <mpi.h>
#include <stdio.h>

#include "./Matrix/Matrix.hpp"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if(id == 0) {
        Matrix m = Matrix(4, 4);

        for(int i = 0; i < 16; i++) {
            m.getData()[i] = i;
        }

        printf("Master Matrix\n");
        m.print();

        MPI_Send(m.getData(), m.getLength(), MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    } else {
        double* data = (double*) calloc(16, sizeof(double));

        Matrix m(data, 2, 2);
        Matrix m2(&(data[4]), 2, 2);

        MPI_Recv(data, 16, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Slave\n");
        m2.print();
    }

    MPI_Finalize();
}