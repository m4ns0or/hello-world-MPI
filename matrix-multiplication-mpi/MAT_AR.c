#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define ARRAY_SIZE 4
#define MATRIX_SIZE 4

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, i, j, sum;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[ARRAY_SIZE], B[MATRIX_SIZE][MATRIX_SIZE], C[ARRAY_SIZE];
    int *localA, *localC;

    srand(time(NULL)); 

    // Fill A with random values on rank 0
    if (rank == 0) {
        printf("Array A:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            A[i] = rand() % 10;
            printf("%d ", A[i]);
        }
        printf("\n");

        printf("Matrix B:\n");
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                B[i][j] = rand() % 10;
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }

    localA = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));
    localC = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));

    // Scatter A
    MPI_Scatter(A, ARRAY_SIZE / size, MPI_INT, localA, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);


    // Print localA for each processor
    // printf("Rank %d: localA\n", rank);
    // for (i = 0; i < ARRAY_SIZE / size; i++) {
    //     printf("%d ", localA[i]);
    // }
    // printf("\n");

    // Broadcast B
    MPI_Bcast(B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Print B for each processor
    // printf("Rank %d: B\n", rank);
    // for (i = 0; i < MATRIX_SIZE; i++) {
    //     for (j = 0; j < MATRIX_SIZE; j++) {
    //         printf("%d ", B[i][j]);
    //     }
    //     printf("\n");
    // }

    // Perform matrix-vector multiplication
    for (i = 0; i < ARRAY_SIZE / size; i++) {
        sum = 0;
        for (j = 0; j < MATRIX_SIZE; j++) {
            sum += localA[i] * B[j][rank + i];
        }
        localC[i] = sum;
    }
    // Print localC for each processor
    // printf("Rank %d: localC\n", rank);
    // for (i = 0; i < ARRAY_SIZE / size; i++) {
    //     printf("%d ", localC[i]);
    // }
    // printf("\n");

    // Gather C
    MPI_Gather(localC, ARRAY_SIZE / size, MPI_INT, C, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print result
    if (rank == 0) {
        printf("Result C:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", C[i]);
        }
        printf("\n");
    }

    free(localA);
    free(localC);

    MPI_Finalize();

    return 0;
}