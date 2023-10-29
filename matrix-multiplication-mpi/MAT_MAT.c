#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MATRIX_SIZE 4

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, i, j, k, sum;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[MATRIX_SIZE][MATRIX_SIZE], B[MATRIX_SIZE][MATRIX_SIZE], C[MATRIX_SIZE][MATRIX_SIZE];
    int** localA = (int**)malloc((MATRIX_SIZE / size) * sizeof(int*));
    int** localC = (int**)malloc((MATRIX_SIZE / size) * sizeof(int*));

    for (i = 0; i < MATRIX_SIZE / size; i++) {
        localA[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
        localC[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
    }

    srand(time(NULL));

    // fill A and B with random values
    if (rank == 0) {
        printf("Matrix A:\n");
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }

        printf("Matrix B:\n");
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                B[i][j] = rand() % 10;
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }

    // scatter A
    MPI_Scatter(A, MATRIX_SIZE * MATRIX_SIZE / size, MPI_INT,
                localA[0], MATRIX_SIZE * MATRIX_SIZE / size, MPI_INT,
                0, MPI_COMM_WORLD);

    //  print localA in each processor
    printf("Rank %d: localA\n", rank);
    for ( i = 0; i < MATRIX_SIZE/size; i++) {
        for ( j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", localA[i][j]);
        }
        printf("\n");
    }
    

    // broadcast B
    MPI_Bcast(B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // print B in each processor
    printf("Rank %d: B\n", rank);
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // perform multiplication
    for (i = 0; i < MATRIX_SIZE / size; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            sum = 0;
            for (k = 0; k < MATRIX_SIZE; k++) {
                sum += localA[i][k] * B[k][j];
            }
            localC[i][j] = sum;
        }
    }

    // print local_c results
    printf("Process %d:\n", rank);
    for (i = 0; i < MATRIX_SIZE / size; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", localC[i][j]);
        }
        printf("\n");
    }

    // gather C
    MPI_Gather(localC[0], MATRIX_SIZE * MATRIX_SIZE / size, MPI_INT,
               C, MATRIX_SIZE * MATRIX_SIZE / size, MPI_INT,
               0, MPI_COMM_WORLD);

    // print result
    if (rank == 0) {
        printf("Matrix C :\n");
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    // Free dynamically allocated memory for localA and localC
    for (i = 0; i < MATRIX_SIZE / size; i++) {
        free(localA[i]);
        free(localC[i]);
    }
    free(localA);
    free(localC);

    MPI_Finalize();

    return 0;
}
