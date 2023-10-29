#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size, snd_buf, recv_buf = 0, sum = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  snd_buf = rank;

  //MPI request variable for the non-blocking receive
  MPI_Request recv_request;

  do {
    // Non-blocking receive
    MPI_Irecv(&recv_buf, 1, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD, &recv_request);

    // Synchronous send
    MPI_Ssend(&snd_buf, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    // Wait for the receive to complete
    MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

    sum += recv_buf;
    snd_buf = recv_buf;
  } while (snd_buf != rank);

  printf("ranks: %d, sum: %d\n", rank, sum);

  MPI_Finalize();
  return 0;
}