#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <unistd.h> 
#include <sys/wait.h>  
#include <sys/shm.h>    

void init_matrix_random(int*, int);
void multiply_matrix_section(int*, int*, int*, int, int , int);

#endif