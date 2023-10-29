#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 4
#define MATRIX_SIZE 4

int main() {
    int i, j, sum;
    int *A, **B, *C;
    int size = ARRAY_SIZE; 

    srand(time(NULL));

    A = (int*)malloc(ARRAY_SIZE * sizeof(int));
    B = (int**)malloc(MATRIX_SIZE * sizeof(int*));
    C = (int*)malloc(ARRAY_SIZE * sizeof(int));

    // allocates memory for each row of the 2D array
    for (i = 0; i < MATRIX_SIZE; i++) {
        B[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
    }

    printf("Vector:\n");
    for (i = 0; i < ARRAY_SIZE; i++) {
        A[i] = rand() % 10;
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Matrix :\n");
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            B[i][j] = rand() % 10;
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < ARRAY_SIZE; i++) {
        sum = 0;
        for (j = 0; j < MATRIX_SIZE; j++) {
            sum += A[j] * B[j][i];
        }
        C[i] = sum;
    }

    printf("Result :\n");
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");


    free(A);
    for (i = 0; i < MATRIX_SIZE; i++) {
        free(B[i]);
    }
    free(B);
    free(C);

    return 0;
}
