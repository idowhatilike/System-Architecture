#include <iostream>
#include <vector>
#include <chrono>

const size_t ARRAY_SIZE = 64 * 1024 * 1024;

void run_stride(const std::vector<int>& arr, size_t stride) {
    long long sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < arr.size(); i += stride) {
        sum += arr[i];
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Stride: " << stride << " | Time: " << elapsed.count() << " ms | Sum: " << sum << std::endl;
}

int main(int argc, char* argv[]) {
    size_t stride = (argc > 1) ? std::stoull(argv[1]) : 1;
    std::vector<int> data(ARRAY_SIZE, 1);
    run_stride(data, stride);
    return 0;
}

/*

Ty le miss gan nhu bang nhau (N / 16)
Sum gan nhu gap 16 lan khi so stride 1 voi 16 va instruction nhieu hon

*/
