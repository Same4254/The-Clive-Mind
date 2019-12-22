#include <mpi.h>
#include <stdio.h>

#include "./Matrix/Matrix.hpp"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int nodeCount;
    MPI_Comm_size(MPI_COMM_WORLD, &nodeCount);

    printf("%d\n", nodeCount);

    double* data = (double*) calloc(10, sizeof(double));

    if(id == 0) {
        MPI_Reduce(MPI_IN_PLACE, data, 10, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        // printf("%f\n", data[0]);
    } else {
        data[0] = 1;
        data[1] = 2;

        MPI_Reduce(data, NULL, 10, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}