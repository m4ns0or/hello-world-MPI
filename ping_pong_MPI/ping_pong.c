#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (num_procs != 2) {
        printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    double start_time, end_time;
    double transfer_time_seconds, transfer_time_microseconds;

    if (my_rank == 0) {
        int ping = 0;
        start_time = MPI_Wtime();

        for (int i = 0; i < 50; i++) {
            MPI_Send(&ping, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

            MPI_Recv(&ping, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        end_time = MPI_Wtime();

        transfer_time_seconds = end_time - start_time;
        transfer_time_microseconds = transfer_time_seconds * 1e6;

        printf("Transfer time (seconds): %lf\n", transfer_time_seconds);
        printf("Transfer time (microseconds): %lf\n", transfer_time_microseconds);
        
    } else if (my_rank == 1) {

  
        int pong;

        for (int i = 0; i < 50; i++) {
            MPI_Recv(&pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(&pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}