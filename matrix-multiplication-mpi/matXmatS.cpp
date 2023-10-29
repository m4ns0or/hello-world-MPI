#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MATRIX_SIZE = 4;

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    int** A = new int*[MATRIX_SIZE];
    int** B = new int*[MATRIX_SIZE];
    int** C = new int*[MATRIX_SIZE];

    // Allocate memory for matrices A, B, and C
    for (int i = 0; i < MATRIX_SIZE; i++) {
        A[i] = new int[MATRIX_SIZE];
        B[i] = new int[MATRIX_SIZE];
        C[i] = new int[MATRIX_SIZE];
    }

    // Fill A and B with random values and perform matrix multiplication
    cout << "Matrix A:\n";
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "Matrix B:\n";
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << B[i][j] << " ";
        }
        cout << "\n";
    }

    // Perform matrix multiplication
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Print result
    cout << "Matrix C:\n";
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << C[i][j] << " ";
        }
        cout << "\n";
    }

    // Deallocate dynamically allocated memory for matrices
    for (int i = 0; i < MATRIX_SIZE; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}