#include "lib.h"


void init_matrix_random(int* matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i * N + j] = std::rand() % 1000;
        }
    }
}

int main() {
    int N;
    int n; 

    std::cout << "Введите размер матрицы N (NxN): ";
    std::cin >> N;
    std::cout << "Введите количество потоков n: ";
    std::cin >> n;

    std::srand(std::time(0));

    int* A = new int[N * N];
    int* B = new int[N * N];
    int* C = new int[N * N]; 

    init_matrix_random(A, N);
    init_matrix_random(B, N);

    std::vector<std::thread> threads; 
    int rows_per_thread = N / n;
    int remaining_rows = N % n;
    int current_row = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i) {
        int start_row = current_row;
        int end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
        
        threads.emplace_back([&, start_row, end_row]() {
            for (int row = start_row; row < end_row; ++row) {
                for (int j = 0; j < N; ++j) {
                    int sum = 0;
                    for (int k = 0; k < N; ++k) {
                        sum += A[row * N + k] * B[k * N + j];
                    }
                    C[row * N + j] = sum;
                }
            }
        });
        current_row = end_row;
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Умножение матриц завершено с использованием " << n << " потоков." << std::endl;
    std::cout << "Время выполнения кода: " << duration_ms.count() << " миллисекунд." << std::endl;

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
