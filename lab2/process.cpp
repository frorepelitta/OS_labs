#include "process.h"


void init_matrix_random(int* matrix, int N) {
    for (int i = 0; i < N; ++i) {
        matrix[i] = std::rand() % 1000;
    }
}

void multiply_matrix_section(int* A, int* B, int* C, int N, int start_row, int end_row) {
    for (int row = start_row; row < end_row; ++row) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[row * N + k] * B[k * N + j];
            }
            C[row * N + j] = sum;
        }
    }
}

int main() {
    int N, n;
    
    std::cout << "Введите размер матрицы N (NxN): ";
    std::cin >> N;
    std::cout << "Введите количество процессов n: ";
    std::cin >> n;

    std::srand(std::time(0));

    int shmidA, shmidB, shmidC;

    shmidA = shmget(IPC_PRIVATE, N * N * sizeof(int), IPC_CREAT | 0666);
    shmidB = shmget(IPC_PRIVATE, N * N * sizeof(int), IPC_CREAT | 0666);
    shmidC = shmget(IPC_PRIVATE, N * N * sizeof(int), IPC_CREAT | 0666);

    int* A = (int*)shmat(shmidA, NULL, 0);
    int* B = (int*)shmat(shmidB, NULL, 0);
    int* C = (int*)shmat(shmidC, NULL, 0);

    init_matrix_random(A, N * N);
    init_matrix_random(B, N * N);
    for (int i = 0; i < N*N; ++i) C[i] = 0;

    int rows_per_proc = N / n;
    int remaining_rows = N % n;
    int current_row = 0;
    pid_t pid;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i) {
        int start = current_row;
        int end = current_row + rows_per_proc + (i < remaining_rows ? 1 : 0);

        pid = fork();

        if (pid == 0) {
            multiply_matrix_section(A, B, C, N, start, end);
            std::exit(0); 
        } else if (pid > 0) {
            current_row = end;
        }
    }

    for (int i = 0; i < n; ++i) {
        wait(NULL); 
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Умножение матриц завершено с использованием " << n << " процессов" << std::endl;
    std::cout << "Время выполнения кода: " << duration_ms.count() << " миллисекунд." << std::endl;

    shmctl(shmidA, IPC_RMID, NULL);
    shmctl(shmidB, IPC_RMID, NULL);
    shmctl(shmidC, IPC_RMID, NULL);

    return 0;
}
