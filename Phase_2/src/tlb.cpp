#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <chrono>

const int MAXLOOPS = 1836;
const int MAXNPAGES = 8;

volatile int a[1024 * 1024 + 1];

int main(){

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(0 , &cpuset);
    // sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

    const auto jump = sysconf(_SC_PAGESIZE) / sizeof(int);

    // Touch each page before timing to exclude first-write costs.
    for (std::size_t j = 0; j < MAXNPAGES * jump; j += jump) {
        a[j] = 0;
    }

    auto start = std::chrono::steady_clock::now();
    for (int i = 0 ; i < MAXLOOPS ; i ++){
        for (std::size_t j = 0; j < MAXNPAGES * jump; j += jump) {
            a[j] += 367;
        }
    }
    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << '\n';

}