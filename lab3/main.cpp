#include "header.h"

int main() {
    const size_t SIZE = 100 * 1024 * 1024;
    const int NUM_PAGES = 1000;
    const size_t PAGE_SIZE = 4096;
    const char* filename = "file.bin";

    int file = open(filename, O_RDWR | O_CREAT);

    std::srand(std::time(nullptr));
    
    // 2. Быстро заполняем случайными данными
    char buffer[PAGE_SIZE];
    for (size_t i = 0; i < SIZE; i += PAGE_SIZE) {
        for (size_t j = 0; j < PAGE_SIZE; j++) {
            buffer[j] = std::rand() % 256;  // Случайный байт 0-255
        }
        write(file, buffer, PAGE_SIZE);
    }
    
    // 3. MMAP
    char* memory = (char*)mmap(nullptr, SIZE, PROT_READ, MAP_PRIVATE, file, 0);
    close(file);
    
    // 4. Генерируем случайные страницы
    int page_addresses[NUM_PAGES];
    size_t total_pages = SIZE / PAGE_SIZE;
    
    for (int i = 0; i < NUM_PAGES; i++) {
        page_addresses[i] = (std::rand() % total_pages) * PAGE_SIZE;
    }
    
    // 5. ПЕРВЫЙ ЗАМЕР (данные с диска)
    auto start1 = std::chrono::high_resolution_clock::now();
    volatile char sum1 = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        sum1 += memory[page_addresses[i]];  // Исправлено: memory и page_addresses
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    double time1 = std::chrono::duration<double>(end1 - start1).count();
    std::cout << "Первый доступ: " << time1 << " сек" << std::endl;

    // 6. ВТОРОЙ ЗАМЕР (данные в кэше)
    auto start2 = std::chrono::high_resolution_clock::now();
    volatile char sum2 = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        sum2 += memory[page_addresses[i]];  // Исправлено: memory и page_addresses
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    double time2 = std::chrono::duration<double>(end2 - start2).count();
    std::cout << "Второй доступ: " << time2 << " сек" << std::endl;

    // 7. Результат
    std::cout << "Ускорение: " << time1 / time2 << " раз" << std::endl;
    
    // 7. Очистка
    munmap(memory, SIZE);
    unlink("file.bin");
    
    return 0;
}