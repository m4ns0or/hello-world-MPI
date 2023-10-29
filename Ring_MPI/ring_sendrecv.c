#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct {
        int rank_int;
        float rank_float;
    } snd_buf, rcv_buf;

    // Initialize the send buffer
    snd_buf.rank_int = rank;
    snd_buf.rank_float = (float)rank;

    // Create a custom datatype for the struct
    MPI_Datatype my_struct_type;
    int blocklengths[2] = {1, 1};
    MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};
    MPI_Aint offsets[2];
    offsets[0] = 0;
    offsets[1] = sizeof(int);
    MPI_Type_create_struct(2, blocklengths, offsets, types, &my_struct_type);
    MPI_Type_commit(&my_struct_type);

    int total_int_sum = 0;
    float total_float_sum = 0.0;

    // Determine the source and destination ranks
    int src_rank = (rank - 1 + size) % size;
    int dest_rank = (rank + 1) % size;

    do {
        MPI_Sendrecv(&snd_buf, 1, my_struct_type, dest_rank, 0, &rcv_buf, 1, my_struct_type, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        total_int_sum += rcv_buf.rank_int;
        total_float_sum += rcv_buf.rank_float;

        snd_buf.rank_int = rcv_buf.rank_int;
        snd_buf.rank_float = rcv_buf.rank_float;
    } while (rcv_buf.rank_int != rank);

    printf("Rank %d: Total Integer Sum of Ranks = %d\n", rank, total_int_sum);
    printf("Rank %d: Total Floating Point Sum of Ranks = %f\n", rank, total_float_sum);

    MPI_Finalize();

    return 0;
}
