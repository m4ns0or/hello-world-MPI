// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>
// #include <time.h>

// #define ARRAY_SIZE 4
// #define MATRIX_SIZE 4

// int main(int argc, char* argv[]) {
//     MPI_Init(&argc, &argv);

//     int rank, size, i, j, sum;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int A[ARRAY_SIZE], B[MATRIX_SIZE][MATRIX_SIZE], C[ARRAY_SIZE];
//     int localA[ARRAY_SIZE / size], localC[ARRAY_SIZE / size];

//     srand(time(NULL)); 

//     // Fill A with random values on rank 0
//     if (rank == 0) {
//         printf("Array A:\n");
//         for (i = 0; i < ARRAY_SIZE; i++) {
//             A[i] = rand() % 10;
//             printf("%d ", A[i]);
//         }
//         printf("\n");

//         printf("Matrix B:\n");
//         for (i = 0; i < MATRIX_SIZE; i++) {
//             for (j = 0; j < MATRIX_SIZE; j++) {
//                 B[i][j] = rand() % 10;
//                 printf("%d ", B[i][j]);
//             }
//             printf("\n");
//         }
//     }

//     // Scatter A
//     MPI_Scatter(A, ARRAY_SIZE / size, MPI_INT, localA, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

//     // Print localA for each processor
//     // printf("Rank %d: localA\n", rank);
//     // for (i = 0; i < ARRAY_SIZE / size; i++) {
//     //     printf("%d ", localA[i]);
//     // }
//     // printf("\n");

//     // Broadcast B
//     MPI_Bcast(B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

//     // Print localB for each processor
//     // printf("Rank %d: localB\n", rank);
//     // for (i = 0; i < MATRIX_SIZE; i++) {
//     //     for (j = 0; j < MATRIX_SIZE; j++) {
//     //         printf("%d ", B[i][j]);
//     //     }
//     //     printf("\n");
//     // }

//     // Perform matrix-vector multiplication
//     for (i = 0; i < ARRAY_SIZE / size; i++) {
//         sum = 0;
//         for (j = 0; j < MATRIX_SIZE; j++) {
//             sum += localA[i] * B[j][rank * (ARRAY_SIZE / size) + i];        }
//         localC[i] = sum;
//     }

//     // Print 
//     // localC for each processor
//     // printf("Rank %d: localC\n", rank);
//     // for (i = 0; i < ARRAY_SIZE / size; i++) {
//     //     printf("%d ", localC[i]);
//     // }
//     // printf("\n");

//     // Gather C
//     MPI_Gather(localC, ARRAY_SIZE / size, MPI_INT, C, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

//     // Print result
//     if (rank == 0) {
//         printf("Result C:\n");
//         for (i = 0; i < ARRAY_SIZE; i++) {
//             printf("%d ", C[i]);
//         }
//         printf("\n");
//     }

//     MPI_Finalize();

//     return 0;
// }


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

    // Declare pointers for dynamic arrays
    int *A = NULL, **B = NULL, *C = NULL, *localA = NULL, *localC = NULL;

    srand(time(NULL));

    // Allocate memory for A, C, localA, and localC
    if (rank == 0) {
        A = (int*)malloc(ARRAY_SIZE * sizeof(int));
        C = (int*)malloc(ARRAY_SIZE * sizeof(int));
    }
    localA = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));
    localC = (int*)malloc((ARRAY_SIZE / size) * sizeof(int));

    // Allocate memory for B as a dynamic 2D array
    B = (int**)malloc(MATRIX_SIZE * sizeof(int*));
    for (i = 0; i < MATRIX_SIZE; i++) {
        B[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
    }

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

    // Scatter A
    MPI_Scatter(A, ARRAY_SIZE / size, MPI_INT, localA, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print localA for each processor
    printf("Rank %d: localA\n", rank);
    for (i = 0; i < ARRAY_SIZE / size; i++) {
        printf("%d ", localA[i]);
    }
    printf("\n");

    // Broadcast B
    for (i = 0; i < MATRIX_SIZE; i++) {
        MPI_Bcast(B[i], MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Print localB for each processor
    printf("Rank %d: localB\n", rank);
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // Perform matrix-vector multiplication
    for (i = 0; i < ARRAY_SIZE / size; i++) {
        sum = 0;
        for (j = 0; j < MATRIX_SIZE; j++) {
            sum += localA[i] * B[j][rank * (ARRAY_SIZE / size) + i];
        }
        localC[i] = sum;
    }

    // Print localC for each processor
    printf("Rank %d: localC\n", rank);
    for (i = 0; i < ARRAY_SIZE / size; i++) {
        printf("%d ", localC[i]);
    }
    printf("\n");

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

    // Free dynamically allocated memory
    if (rank == 0) {
        free(A);
        free(C);
    }
    free(localA);
    free(localC);
    for (i = 0; i < MATRIX_SIZE; i++) {
        free(B[i]);
    }
    free(B);

    MPI_Finalize();

    return 0;
}
