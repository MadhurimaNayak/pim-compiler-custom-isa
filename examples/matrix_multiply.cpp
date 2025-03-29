#include <iostream>
#include <vector>

// Simple matrix multiplication function
void matrix_multiply(int* A, int* B, int* C, int A_rows, int A_cols, int B_cols) {
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            C[i * B_cols + j] = 0;
            for (int k = 0; k < A_cols; k++) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

int main() {
    const int M = 4;  // A rows
    const int N = 4;  // A cols, B rows
    const int P = 4;  // B cols
    
    // Initialize matrices
    std::vector<int> A(M * N);
    std::vector<int> B(N * P);
    std::vector<int> C(M * P);
    
    // Fill matrices A and B with some values
    for (int i = 0; i < M * N; i++) {
        A[i] = i + 1;
    }
    
    for (int i = 0; i < N * P; i++) {
        B[i] = i + 1;
    }
    
    // Multiply matrices
    matrix_multiply(A.data(), B.data(), C.data(), M, N, P);
    
    // Print result
    std::cout << "Result Matrix C:" << std::endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            std::cout << C[i * P + j] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
