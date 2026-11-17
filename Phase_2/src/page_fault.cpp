#include <iostream>
#include <vector>
#include <cstring>

int main() {
    const size_t SIZE = 100 * 1024 * 1024; // 100 MB
    std::cout << "Cấp phát 100 MB bộ nhớ..." << std::endl;
    volatile char* mem = new char[SIZE];

    std::cout << "Bắt đầu chạm vào từng trang 4KB để kích hoạt Demand Paging..." << std::endl;
    for (size_t i = 0; i < SIZE; i += 4096) {
        mem[i] = 'A'; 
    }
    
    delete[] mem;
    return 0;
}
