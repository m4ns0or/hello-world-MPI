#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int size, rank; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    // Synchronize all processes
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        int message = 0; 

        printf("I am 0, out of %d\n", size);
        printf("Hello, World!\n");

        if (size > 1) {
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        int message;

        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("I am out %d of %d\n", rank, size);

        if (rank < size - 1) {
            message = rank; 
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}