#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <chrono>

int main() {
    const int ITERATIONS = 100000;
    struct timeval start, end;

    gettimeofday(&start, nullptr);
    auto Start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
        read(STDIN_FILENO, nullptr, 0);
    auto End = std::chrono::steady_clock::now();
    gettimeofday(&end, nullptr);

    double total_us = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    std::chrono::duration<double , std::micro> total_time = End - Start; 
    double ret = total_time.count() / ITERATIONS;

    std::cout << "1 System Call : "  << (total_us / ITERATIONS) << " us\n || " << ret << " us\n";
    return 0;
}