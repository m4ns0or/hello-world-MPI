#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int i, sum;
    int ARRAY_SIZE = 8;

    int* A = NULL, * B = NULL, * C = NULL;
    int* localA = NULL, * localB = NULL, * localC = NULL;

    srand(time(NULL));

    // Fill A and B with random values
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

    // Allocate memory for local arrays
    localA = (int*)malloc(ARRAY_SIZE * sizeof(int));
    localB = (int*)malloc(ARRAY_SIZE * sizeof(int));
    localC = (int*)malloc(ARRAY_SIZE * sizeof(int));

    // Copy A and B to localA and localB
    for (i = 0; i < ARRAY_SIZE; i++) {
        localA[i] = A[i];
        localB[i] = B[i];
    }

    // Perform element-wise multiplication
    for (i = 0; i < ARRAY_SIZE; i++) {
        localC[i] = localA[i] * localB[i];
    }

    // Print result
    printf("Result C:\n");
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", localC[i]);
    }
    printf("\n");

    // Free memory for A, B, and C
    free(A);
    free(B);
    free(localA);
    free(localB);
    free(localC);

    return 0;
}
