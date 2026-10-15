#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sched.h>
#include <chrono>

int main() {
    const int ITERATIONS = 100000;
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    pid_t pid = fork();

    if (pid == 0) { 
        sched_setaffinity(0, sizeof(cpu_set_t), &set);
        char buf = 'a';
        for (int i = 0; i < ITERATIONS; ++i) {
            read(pipe1[0], &buf, 1);
            write(pipe2[1], &buf, 1);
        }
        return 0;
    } else { 
        sched_setaffinity(0, sizeof(cpu_set_t), &set);
        char buf = 'a';
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            write(pipe1[1], &buf, 1);
            read(pipe2[0], &buf, 1);  
        }
        auto end = std::chrono::steady_clock::now();
        wait(nullptr);

        std::chrono::duration<double , std::micro> total_time = end - start; 
        double ret = total_time.count() / ITERATIONS;
        
        std::cout << "1 Context Switch : " << ret << " us\n";
    }
    return 0;
}