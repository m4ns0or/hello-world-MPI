#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int snd_buf = rank;
    int rcv_buf;
    int total_sum = 0;

    MPI_Request send_request, recv_request;
    MPI_Status recv_status;

    // Start the non-blocking send
    MPI_Issend(&snd_buf, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_request);

    do {
        MPI_Recv(&rcv_buf, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &recv_status);

        total_sum += rcv_buf;

        // Wait for the non-blocking send to complete
        MPI_Wait(&send_request, MPI_STATUS_IGNORE);
        
        MPI_Issend(&rcv_buf, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_request);

        //current process's rank, 
    } while (rcv_buf != rank);

    printf("Rank %d: Total Sum of Ranks is  = %d\n", rank, total_sum);

    MPI_Finalize();

    return 0;
}