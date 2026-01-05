#include "header.h"

int main() {
    const int SIZE = 100 * 1024 * 1024;
    const int NUM_PAGES = 1000;
    const int PAGE_SIZE = 4096;
    const char* filename = "file.bin";

    int file = open(filename, O_RDWR | O_CREAT);

    std::srand(std::time(nullptr));
    
    char buffer[PAGE_SIZE];
    for (int i = 0; i < SIZE; i += PAGE_SIZE) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            buffer[j] = std::rand() % 256; 
        }
        write(file, buffer, PAGE_SIZE);
    }
    
    char* memory = (char*)mmap(nullptr, SIZE, PROT_READ, MAP_PRIVATE, file, 0);
    close(file);
    
    // генерация очереди для случайного обращения к страницам
    int page_addresses[NUM_PAGES];
    int total_pages = SIZE / PAGE_SIZE;
    
    for (int i = 0; i < NUM_PAGES; i++) {
        page_addresses[i] = (std::rand() % total_pages) * PAGE_SIZE;
    }
    
    // первое обращение
    struct rusage usage1_before, usage1_after;
    getrusage(RUSAGE_SELF, &usage1_before);

    auto start1 = std::chrono::high_resolution_clock::now();
    volatile char sum1 = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        sum1 += memory[page_addresses[i]]; 
    }

    getrusage(RUSAGE_SELF, &usage1_after);

    auto end1 = std::chrono::high_resolution_clock::now();
    double time1 = std::chrono::duration<double>(end1 - start1).count();

    long major_faults_1 = usage1_after.ru_majflt - usage1_before.ru_majflt;
    long minor_faults_1 = usage1_after.ru_minflt - usage1_before.ru_minflt;

    std::cout << "Первое обращение: " << time1 << " сек" << std::endl;
    std::cout << "Major faults: " << major_faults_1 << std::endl;
    std::cout << "Minor faults: " << minor_faults_1 << std::endl;

    // повторное обращение
    struct rusage usage2_before, usage2_after;
    getrusage(RUSAGE_SELF, &usage2_before); 

    auto start2 = std::chrono::high_resolution_clock::now();
    volatile char sum2 = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        sum2 += memory[page_addresses[i]]; 
    }

    getrusage(RUSAGE_SELF, &usage2_after); 

    auto end2 = std::chrono::high_resolution_clock::now();
    double time2 = std::chrono::duration<double>(end2 - start2).count();

    long major_faults_2 = usage2_after.ru_majflt - usage2_before.ru_majflt;
    long minor_faults_2 = usage2_after.ru_minflt - usage2_before.ru_minflt;

    std::cout << "Второе обращение: " << time2 << " сек" << std::endl;
    std::cout << "  Major faults: " << major_faults_2 << std::endl;
    std::cout << "  Minor faults: " << minor_faults_2 << std::endl;
    std::cout << "Ускорение: " << time1 / time2 << " раз" << std::endl;

    munmap(memory, SIZE);
    unlink("file.bin");
    
    return 0;
}