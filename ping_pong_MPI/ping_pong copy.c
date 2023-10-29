#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define ITERATIONS 50

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

    int message_sizes[] = {8, 512, 32 * 1024, 2 * 1024 * 1024}; 
    MPI_Datatype data_types[] = {MPI_CHAR, MPI_CHAR, MPI_CHAR, MPI_CHAR}; 

    for (int i = 0; i < 4; i++) {
        int message_size = message_sizes[i];
        MPI_Datatype data_type = data_types[i];

        double start_time, end_time;
        double standard_transfer_time, synchronous_transfer_time;

        // Prepare data to send
        char* data_send = (char*)malloc(message_size);
        char* data_recv = (char*)malloc(message_size);

        MPI_Barrier(MPI_COMM_WORLD);

        start_time = MPI_Wtime();

        if (my_rank == 0) {
            for (int j = 0; j < ITERATIONS; j++) {
                MPI_Send(data_send, message_size, data_type, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data_recv, message_size, data_type, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            for (int j = 0; j < ITERATIONS; j++) {
                MPI_Recv(data_recv, message_size, data_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data_send, message_size, data_type, 0, 0, MPI_COMM_WORLD);
            }
        }

        end_time = MPI_Wtime();
        standard_transfer_time = (end_time - start_time) / (2.0 * ITERATIONS); 

        start_time = MPI_Wtime();

        if (my_rank == 0) {
            for (int j = 0; j < ITERATIONS; j++) {
                MPI_Ssend(data_send, message_size, data_type, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data_recv, message_size, data_type, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            for (int j = 0; j < ITERATIONS; j++) {
                MPI_Recv(data_recv, message_size, data_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(data_send, message_size, data_type, 0, 0, MPI_COMM_WORLD);
            }
        }

        end_time = MPI_Wtime();
        synchronous_transfer_time = (end_time - start_time) / (2.0 * ITERATIONS); 

        free(data_send);
        free(data_recv);

        if (my_rank == 0) {
            double bandwidth_standard = (double)message_size / standard_transfer_time;
            double bandwidth_synchronous = (double)message_size / synchronous_transfer_time;

            printf("\nMessage Size: %d bytes\n", message_size);
            printf("Standard Transfer Time: %lf seconds\n", standard_transfer_time);
            printf("Standard Bandwidth: %lf bytes/second\n", bandwidth_standard);
            printf("Synchronous Transfer Time: %lf seconds\n", synchronous_transfer_time);
            printf("Synchronous Bandwidth: %lf bytes/second\n", bandwidth_synchronous);
        }
    }

    MPI_Finalize();
    return 0;
}
