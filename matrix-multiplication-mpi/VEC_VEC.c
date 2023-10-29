#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, i, sum;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int ARRAY_SIZE = 8; 


    int* A = NULL, * B = NULL, * C = NULL;
    int* localA = NULL, * localB = NULL, * localC = NULL;

    srand(time(NULL));

    // Fill A and B with random values on rank 0
    if (rank == 0) {
        A = (int*)malloc(ARRAY_SIZE * sizeof(int));
        B = (int*)malloc(ARRAY_SIZE * sizeof(int));
        printf("Array A:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            A[i] = rand() % 10;
            printf("%d ", A[i]);
        }
        printf("\n");

        printf("Array B:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            B[i] = rand() % 10;
            printf("%d ", B[i]);
        }
        printf("\n");
    }

    // Allocate memory for local arrays
    localA = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));
    localB = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));
    localC = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));

    // Scatter A and B
    MPI_Scatter(A, ARRAY_SIZE / size, MPI_INT, localA, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, ARRAY_SIZE / size, MPI_INT, localB, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print localA and localB in each processor
    // printf("Rank %d: localA\n", rank);
    // for (i = 0; i < ARRAY_SIZE / size; i++) {
    //     printf("%d ", localA[i]);
    // }
    // printf("\n");

    // printf("Rank %d: localB\n", rank);
    // for (i = 0; i < ARRAY_SIZE / size; i++) {
    //     printf("%d ", localB[i]);
    // }
    // printf("\n");

    // Perform element-wise multiplication
    for (i = 0; i < ARRAY_SIZE / size; i++) {
        localC[i] = localA[i] * localB[i];
    }

    // Print localC results
    // printf("Rank %d: localC\n", rank);
    // for (i = 0; i < ARRAY_SIZE / size; i++) {
    //     printf("%d ", localC[i]);
    // }
    // printf("\n");

    // Gather C
    if (rank == 0) {
        C = (int*)malloc(ARRAY_SIZE * sizeof(int));
    }
    MPI_Gather(localC, ARRAY_SIZE / size, MPI_INT, C, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print result
    if (rank == 0) {
        printf("Result C:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", C[i]);
        }
        printf("\n");

        // Free memory for A, B, and C on rank 0
        free(A);
        free(B);
        free(C);
    }

    // Free memory for local arrays
    free(localA);
    free(localB);
    free(localC);

    MPI_Finalize();

    return 0;
}
