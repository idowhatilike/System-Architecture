#include <iostream>
#include <unistd.h>
#include <vector>

int main() {
    int pfd[2];
    pipe(pfd); // pfd[0]: read, pfd[1]: write
    
    std::vector<char> chunk(4096, 'X'); // Khối 4 KB
    size_t total_written = 0;

    std::cout << "[Producer] Bắt đầu ghi liên tục vào pipe mà không đọc..." << std::endl;
    while (true) {
        ssize_t bytes = write(pfd[1], chunk.data(), chunk.size());
        total_written += bytes;
        std::cout << "Đã ghi thành công: " << total_written << " bytes (khối tiếp theo sẽ ghi...)" << std::endl;
    }
    return 0;
}
