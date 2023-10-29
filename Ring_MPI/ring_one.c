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

    // Offsets are expressed in bytes and indicate the byte positions where each element in the structured data type begins with respect to the start of the data structure.
    // they allow you to specify the layout of non-contiguous data within the custom datatype.

    MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};
    MPI_Aint offsets[2];
    offsets[0] = 0;
    offsets[1] = sizeof(int);
    MPI_Type_create_struct(2, blocklengths, offsets, types, &my_struct_type);
    MPI_Type_commit(&my_struct_type);

    int total_int_sum = 0;
    float total_float_sum = 0.0;

    MPI_Request send_request, recv_request;
    MPI_Status recv_status;

    // Start the non-blocking send
    MPI_Issend(&snd_buf, 1, my_struct_type, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_request);

    do {
        MPI_Recv(&rcv_buf, 1, my_struct_type, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &recv_status);

        total_int_sum += rcv_buf.rank_int;
        total_float_sum += rcv_buf.rank_float;

        // Wait for the non-blocking send
        MPI_Wait(&send_request, MPI_STATUS_IGNORE);

        snd_buf.rank_int = rcv_buf.rank_int;
        snd_buf.rank_float = rcv_buf.rank_float;
        MPI_Issend(&snd_buf, 1, my_struct_type, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_request);
    } while (rcv_buf.rank_int != rank);

    printf("Rank %d: Total Integer Sum of Ranks = %d\n", rank, total_int_sum);
    printf("Rank %d: Total Floating Point Sum of Ranks = %f\n", rank, total_float_sum);

    MPI_Finalize();

    return 0;
}

//  mpicc aaa.c -o aaa   
//  mpiexec -n 4 ./aaa    